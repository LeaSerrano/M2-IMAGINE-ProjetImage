
import torch
import torch.nn.functional as F
from runpy import run_path
from skimage import img_as_ubyte
import cv2
import os
import sys

def process_single_image(input_filepath, out_filepath):
    isCudaAvailable = torch.cuda.is_available()

    def get_weights_and_parameters(task, parameters):
        if task == 'Real_Denoising':
            weights = '/home/e20190003227/M2-IMAGINE/Image/HAI927-Projet_IMAGE/code/model/real_denoising.pth'
            parameters['LayerNorm_type'] = 'BiasFree'
        return weights, parameters

    # Get model weights and parameters
    parameters = {'inp_channels':3, 'out_channels':3, 'dim':48, 'num_blocks':[4,6,6,8], 'num_refinement_blocks':4, 'heads':[1,2,4,8], 'ffn_expansion_factor':2.66, 'bias':False, 'LayerNorm_type':'WithBias', 'dual_pixel_task':False}
    weights, parameters = get_weights_and_parameters('Real_Denoising', parameters)

    # Load model architecture
    load_arch = run_path('/home/e20190003227/M2-IMAGINE/Image/HAI927-Projet_IMAGE/code/model/restormer_arch.py')
    model = load_arch['Restormer'](**parameters)

    if isCudaAvailable:
        model.cuda()
    else:
        model.cpu()

    # Load model weights
    checkpoint = torch.load(weights)
    model.load_state_dict(checkpoint['params'])
    model.eval()

    img = cv2.cvtColor(cv2.imread(input_filepath), cv2.COLOR_BGR2RGB)
    input_ = torch.from_numpy(img).float().div(255.).permute(2, 0, 1).unsqueeze(0)

    # Pad the input if not_multiple_of 8
    img_multiple_of = 8
    h, w = input_.shape[2], input_.shape[3]
    H, W = ((h + img_multiple_of) // img_multiple_of) * img_multiple_of, ((w + img_multiple_of) // img_multiple_of) * img_multiple_of
    padh = H - h if h % img_multiple_of != 0 else 0
    padw = W - w if w % img_multiple_of != 0 else 0
    input_ = F.pad(input_, (0, padw, 0, padh), 'reflect')

    with torch.no_grad():
        restored = model(input_)
        restored = torch.clamp(restored, 0, 1)

        # Unpad the output
        restored = restored[:, :, :h, :w]

        restored = restored.permute(0, 2, 3, 1).cpu().detach().numpy()
        restored = img_as_ubyte(restored[0])

        cv2.imwrite(out_filepath, cv2.cvtColor(restored, cv2.COLOR_RGB2BGR))

        if not isCudaAvailable:
            del input_, restored

    return out_filepath  # Renvoie le nom du fichier traité


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python process_single_image.py input_filepath output_dir")
        sys.exit(1)

    input_filepath = sys.argv[1]
    out_dir = sys.argv[2]

    processed_file = process_single_image(input_filepath, out_dir)

    print(processed_file)
