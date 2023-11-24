import tensorflow as tf
from tensorflow.keras.layers import Input, Conv2D, Conv2DTranspose, AveragePooling2D, concatenate, Add
from tensorflow.keras.models import Model
from tensorflow.keras.preprocessing import image
import numpy as np
import cv2
import matplotlib.pyplot as plt
import os

tf.keras.backend.clear_session()


train_dir_rgb = 'BD_noisy/RGB/'
train_dir_grayscale = 'BD_noisy/Grayscale/'

#Pour les images en couleur
# Charger toutes les images dans un répertoire
images_rgb = []

for filename in os.listdir(train_dir_rgb):
    img_path = os.path.join(train_dir_rgb, filename)
    img = image.load_img(img_path, target_size=(256, 256))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    images_rgb.append(img_array)

# Concaténer toutes les images pour former l'ensemble de données d'entraînement
x_train_rgb = np.concatenate(images_rgb, axis=0)


#Pour les images niveaux de gris
# Charger toutes les images dans un répertoire
images_grayscale = []

for filename in os.listdir(train_dir_grayscale):
    img_path = os.path.join(train_dir_grayscale, filename)
    img = image.load_img(img_path, target_size=(256, 256), color_mode='grayscale')
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    images_grayscale.append(img_array)

# Concaténer toutes les images pour former l'ensemble de données d'entraînement
x_train_grayscale = np.concatenate(images_grayscale, axis=0)


#Normalisation avec min et max

#Pour les images en couleur
min_val = np.min(x_train_rgb)
max_val = np.max(x_train_rgb)

x_train_normalized_rgb = 2 * (x_train_rgb - min_val) / (max_val - min_val) - 1
y_train_normalized_rgb = x_train_normalized_rgb.copy()

#Pour les images en niveaux de gris
min_val = np.min(x_train_grayscale)
max_val = np.max(x_train_grayscale)

x_train_normalized_grayscale = 2 * (x_train_grayscale - min_val) / (max_val - min_val) - 1
y_train_normalized_grayscale = x_train_grayscale.copy()


def build_cbdnet(input_shape, isAGrayscaleFile):

    if isAGrayscaleFile == True :
      # Noise estimation subnetwork
      input_tensor = Input(shape=input_shape)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(input_tensor)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      noise_estimation = Conv2D(1, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      
      # Non Blind denoising subnetwork
      input_plus_noise = concatenate([input_tensor, noise_estimation])
      conv1 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(input_plus_noise)
      conv2 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv1)
      
      pool1 = AveragePooling2D(pool_size=(2, 2), padding='same')(conv2)
      conv3 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(pool1)
      conv4 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv3)
      conv5 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv4)
      
      pool2 = AveragePooling2D(pool_size=(2, 2), padding='same')(conv5)
      conv6 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(pool2)
      conv7 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv6)
      conv8 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv7)
      conv9 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv8)
      conv10 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv9)
      conv11 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv10)
      
      upsample1 = Conv2DTranspose(128, (3, 3), strides=2, activation="relu", kernel_initializer='he_normal',
                                  padding="same")(conv11)
      add1 = Add()([upsample1, conv5])
      conv12 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(add1)
      conv13 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv12)
      conv14 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv13)
      
      upsample2 = Conv2DTranspose(64, (3, 3), strides=2, activation="relu", kernel_initializer='he_normal',
                                  padding="same")(conv14)
      add2 = Add()([upsample2, conv2])
      conv15 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(add2)
      conv16 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv15)
      
      out = Conv2D(1, (1, 1), kernel_initializer='he_normal', padding="same")(conv16)
      denoised_image = Add()([out, input_tensor])

    elif isAGrayscaleFile == False :
      # Noise estimation subnetwork
      input_tensor = Input(shape=input_shape)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(input_tensor)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      x = Conv2D(32, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)
      noise_estimation = Conv2D(3, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(x)

      # Non Blind denoising subnetwork
      input_plus_noise = concatenate([input_tensor, noise_estimation])
      conv1 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(input_plus_noise)
      conv2 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv1)

      pool1 = AveragePooling2D(pool_size=(2, 2), padding='same')(conv2)
      conv3 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(pool1)
      conv4 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv3)
      conv5 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv4)

      pool2 = AveragePooling2D(pool_size=(2, 2), padding='same')(conv5)
      conv6 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(pool2)
      conv7 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv6)
      conv8 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv7)
      conv9 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv8)
      conv10 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv9)
      conv11 = Conv2D(256, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv10)

      upsample1 = Conv2DTranspose(128, (3, 3), strides=2, activation="relu", kernel_initializer='he_normal',
                                  padding="same")(conv11)
      add1 = Add()([upsample1, conv5])
      conv12 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(add1)
      conv13 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv12)
      conv14 = Conv2D(128, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv13)

      upsample2 = Conv2DTranspose(64, (3, 3), strides=2, activation="relu", kernel_initializer='he_normal',
                                  padding="same")(conv14)
      add2 = Add()([upsample2, conv2])
      conv15 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(add2)
      conv16 = Conv2D(64, (3, 3), activation="relu", kernel_initializer='he_normal', padding="same")(conv15)

      out = Conv2D(3, (1, 1), kernel_initializer='he_normal', padding="same")(conv16)
      denoised_image = Add()([out, input_tensor])

    model = Model(inputs=input_tensor, outputs=denoised_image)
    return model


input_shape_rgb = (256, 256, 3)
input_shape_grayscale = (256, 256, 1)

CBDNet_rgb = build_cbdnet(input_shape_rgb, False)
CBDNet_rgb.compile(optimizer=tf.keras.optimizers.Adam(1e-03), loss=tf.keras.losses.MeanSquaredError())
CBDNet_rgb.summary()

CBDNet_grayscale = build_cbdnet(input_shape_grayscale, True)
CBDNet_grayscale.compile(optimizer=tf.keras.optimizers.Adam(1e-03), loss=tf.keras.losses.MeanSquaredError())
CBDNet_grayscale.summary()



# Pour les images en couleur
CBDNet_rgb.compile(optimizer=tf.keras.optimizers.Adam(1e-03), loss=tf.keras.losses.MeanSquaredError())
CBDNet_rgb.summary()

CBDNet_rgb.fit(x_train_normalized_rgb, y_train_normalized_rgb, epochs=10, batch_size=1)

# Pour les images en niveaux de gris
CBDNet_grayscale.compile(optimizer=tf.keras.optimizers.Adam(1e-03), loss=tf.keras.losses.MeanSquaredError())
CBDNet_grayscale.summary()

CBDNet_grayscale.fit(x_train_normalized_grayscale, y_train_normalized_grayscale, epochs=10, batch_size=1)


# Sauvegarde du modèle
CBDNet_rgb.save("CBDNet_rgb_model.h5")
CBDNet_grayscale.save("CBDNet_grayscale_model.h5")