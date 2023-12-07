import cv2
import niqe

def measure_niqe(image_path):
    img = cv2.imread(image_path)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Use the NIQE model
    niqe_score = niqe.score(img)

    return niqe_score

if __name__ == "__main__":
    image_path = "path/to/your/image.jpg"
    niqe_score = measure_niqe(image_path)
    print(f"NIQE Score for the image: {niqe_score}")
