
import torch
import torch.nn.functional as F
import torchvision.transforms.functional as TF
from runpy import run_path
from skimage import img_as_ubyte
from natsort import natsorted
from glob import glob
import cv2
from tqdm import tqdm
import argparse
import numpy as np
import os

isCudaAvailable = False

if torch.cuda.is_available():
    print("CUDA est disponible sur cet ordinateur.")
    isCudaAvailable = True
else:
    print("CUDA n'est pas disponible sur cet ordinateur. Le code s'exécutera sur le CPU.")

def get_weights_and_parameters(task, parameters):
    if task == 'Real_Denoising':
        weights = 'model/real_denoising.pth'
        parameters['LayerNorm_type'] = 'BiasFree'
    return weights, parameters

# Get model weights and parameters
parameters = {'inp_channels':3, 'out_channels':3, 'dim':48, 'num_blocks':[4,6,6,8], 'num_refinement_blocks':4, 'heads':[1,2,4,8], 'ffn_expansion_factor':2.66, 'bias':False, 'LayerNorm_type':'WithBias', 'dual_pixel_task':False}
weights, parameters = get_weights_and_parameters('Real_Denoising', parameters)

# Load model architecture
load_arch = run_path(os.path.join('model', 'restormer_arch.py'))
model = load_arch['Restormer'](**parameters)
if isCudaAvailable == True : 
  model.cuda()
else : 
  model.cpu()

# Load model weights
checkpoint = torch.load(weights)
model.load_state_dict(checkpoint['params'])
model.eval()

input_dir = 'BD_test'
out_dir = 'BD_result'
os.makedirs(out_dir, exist_ok=True)
extensions = ['jpg', 'JPG', 'png', 'PNG', 'jpeg', 'JPEG', 'bmp', 'BMP']
files = natsorted(glob(os.path.join(input_dir, '*')))

img_multiple_of = 8

print(f"\n ==> Running Real_Denoising with weights {weights}\n ")
with torch.no_grad():
  for filepath in tqdm(files):
      if isCudaAvailable == True : 
        torch.cuda.ipc_collect()
        torch.cuda.empty_cache()
        img = cv2.cvtColor(cv2.imread(filepath), cv2.COLOR_BGR2RGB)
        input_ = torch.from_numpy(img).float().div(255.).permute(2,0,1).unsqueeze(0).cuda()
      else : 
        img = cv2.cvtColor(cv2.imread(filepath), cv2.COLOR_BGR2RGB)
        input_ = torch.from_numpy(img).float().div(255.).permute(2, 0, 1).unsqueeze(0)

      # Pad the input if not_multiple_of 8
      h,w = input_.shape[2], input_.shape[3]
      H,W = ((h+img_multiple_of)//img_multiple_of)*img_multiple_of, ((w+img_multiple_of)//img_multiple_of)*img_multiple_of
      padh = H-h if h%img_multiple_of!=0 else 0
      padw = W-w if w%img_multiple_of!=0 else 0
      input_ = F.pad(input_, (0,padw,0,padh), 'reflect')

      restored = model(input_)
      restored = torch.clamp(restored, 0, 1)

      # Unpad the output
      restored = restored[:,:,:h,:w]

      restored = restored.permute(0, 2, 3, 1).cpu().detach().numpy()
      restored = img_as_ubyte(restored[0])

      filename = os.path.split(filepath)[-1]
      cv2.imwrite(os.path.join(out_dir, filename),cv2.cvtColor(restored, cv2.COLOR_RGB2BGR))
      
      if not isCudaAvailable:
        del input_, restored