import cv2 as cv
import numpy as np

img = cv.imread('F://pj_python//1_jihua//images_prj//bird.png')
blur = cv.blur(img, (5, 5))
blur0 = cv.medianBlur(blur, 5)
blur1 = cv.GaussianBlur(blur0, (5, 5), 0)
blur2 = cv.bilateralFilter(blur1, 9, 75, 75)
hsv = cv.cvtColor(blur2, cv.COLOR_BGR2HSV)
low_blue = np.array([55, 0, 0])
high_blue = np.array([118, 255, 255])
mask = cv.inRange(hsv, low_blue, high_blue)
res = cv.bitwise_and(img, img, mask=mask)
cv.imshow("image", res)
cv.waitKey()
