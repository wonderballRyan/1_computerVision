import cv2
import numpy as np

image_1 = cv2.imread('images/image_1.png', 1)
image_2 = cv2.imread('images/image_2.png', 1)
image_3 = cv2.resize(image_2, dsize=(100, 100))
alpha_image_3 = image_3[:, :] / 255.0
# print(np.unique(alpha_image_3))
alpha_image = 1 - alpha_image_3
for c in range(0, 3):
    image_1[220:320, 180:280] = (
            (alpha_image_3 * image_1[220:320, 180:280]) + (alpha_image * image_3[:, :]))
# image_1[150:250, 150:250] = image_3
cv2.imshow('image', image_1)
cv2.waitKey(0)
