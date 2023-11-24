from tensorflow.keras.models import load_model

# Chargement du modèle
loaded_CBDNet_rgb = load_model("CBDNet_rgb_model.h5")
loaded_CBDNet_grayscale = load_model("CBDNet_grayscale_model.h5")

def denoise_and_display_images(noisy_folder_path, isAGrayscaleFile, target_size=(256, 256)):

        if (isAGrayscaleFile == False) :
            noisy_img = image.load_img(noisy_folder_path, target_size=target_size)
            noisy_img_array = image.img_to_array(noisy_img)
            noisy_img_array = np.expand_dims(noisy_img_array, axis=0)

            denoised_img_array = CBDNet_rgb.predict(noisy_img_array)

        elif (isAGrayscaleFile == True) :
            noisy_img = image.load_img(noisy_folder_path, target_size=target_size, color_mode='grayscale')
            noisy_img_array = image.img_to_array(noisy_img)
            noisy_img_array = np.expand_dims(noisy_img_array, axis=0)

            denoised_img_array = CBDNet_grayscale.predict(noisy_img_array)

        # Affichage de l'image bruitée et débruitée
        plt.figure(figsize=(12, 4))

        # Afficher l'image bruitée
        plt.subplot(1, 2, 1)
        plt.imshow(noisy_img_array[0].astype(np.uint8), cmap='gray' if isAGrayscaleFile else None)
        plt.title("Image bruitée")
        plt.axis("off")

        # Afficher l'image débruitée
        plt.subplot(1, 2, 2)
        plt.imshow(denoised_img_array[0].astype(np.uint8), cmap='gray' if isAGrayscaleFile else None)
        plt.title("Image débruitée")
        plt.axis("off")

        plt.show()


# Chemin vers le dossier contenant les images bruitées
noisy_folder_path_rgb = 'BD_noisy/citrouilles_Speckle_50.png'

# Appel de la fonction pour traiter les images
denoise_and_display_images(noisy_folder_path_rgb, False)


#Images en niveaux de gris

# Chemin vers le dossier contenant les images bruitées
noisy_folder_path_grayscale = 'BD_noisy/citrouilles_PoivreEtSel_0.100000.png'

# Appel de la fonction pour traiter les images
denoise_and_display_images(noisy_folder_path_grayscale, True)