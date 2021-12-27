import cv2
import numpy as np

# read image
bgr_img = cv2.imread("images/color.png")
# convert to HSV
hsv_image = cv2.cvtColor(bgr_img, cv2.COLOR_BGR2HSV)


# lower and higher HSV value for red color
def red_hsv():
    lower_hsv_1 = np.array([0, 175, 20])
    higher_hsv_1 = np.array([10, 255, 255])

    lower_hsv_2 = np.array([170, 175, 20])
    higher_hsv_2 = np.array([180, 255, 255])

    mask_1 = cv2.inRange(hsv_image, lower_hsv_1, higher_hsv_1)
    mask_2 = cv2.inRange(hsv_image, lower_hsv_2, higher_hsv_2)

    return mask_1 + mask_2


def green_hsv():
    lower_hsv = np.array([40, 150, 20])
    higher_hsv = np.array([70, 255, 255])

    mask = cv2.inRange(hsv_image, lower_hsv, higher_hsv)
    return mask


mask = red_hsv() + green_hsv()

detected_img = cv2.bitwise_and(bgr_img, bgr_img, mask=mask)

cv2.imshow("Detected Image", detected_img)
cv2.waitKey()
