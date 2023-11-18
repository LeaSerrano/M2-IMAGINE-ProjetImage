
# prepare executable
g++ Denoise_Data.cpp -o Denoise -O3
g++ Noise_Data.cpp -o Noise -O3
g++ Mesure.cpp -o Mesure -O3

# Use example

# Creates DB/cover_N/GAU_0_1 and fills the directory with the images from DB/cover modified with gaussien noise ( 0, 1 )
sh GenImg.sh cover N GAU 0 1 

# Creates DB/cover_D/GAU_0_1_MOY_1 and fills the directory with the images from DB/cover_N/GAU_0_1 denoised with a mean filter ( 1 )
sh GenImg.sh cover D GAU_0_1 MOY 1 

# Compares the PSNR, SNR, SSIM and RMSE of each corresponding images in DB/cover and DB/cover_D/GAU_0_1_MOY_1
# Stocks the results in four different text files : DB/cover_M/{PSNR|SNR|SSIM|RMSE}_GAU_0_1_MOY_1.txt
sh GenImg.sh cover M GAU_0_1_MOY_1

# Deletes the repositories DB/cover_N/GAU_0_1 and DB/cover_D/GAU_0_1_MOY_1 and their contents
sh GenImg.sh DB/cover_N/GAU_0_1 R
sh GenImg.sh DB/cover_D/GAU_0_1_MOY_1 R

rm Denoise
rm Noise
rm Mesure 