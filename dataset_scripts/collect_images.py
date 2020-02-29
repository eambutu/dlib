from imutils.video import VideoStream
from imutils import face_utils
import os
import argparse
import imutils
import time
import dlib
import cv2

# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("--image-dir", required=True,
    help="path to image directory to write")
args = vars(ap.parse_args())

# initialize the video stream and allow the cammera sensor to warmup
print("[INFO] camera sensor warming up...")
vs = VideoStream(src=0).start()
time.sleep(2.0)

index = 0

# loop over the frames from the video stream
while True:
    # grab the frame from the video stream, resize it to have a
    # maximum width of 400 pixels, and convert it to grayscale
    frame = vs.read()
    frame = imutils.resize(frame, width=400)

    # show the frame
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord("a"):
        cv2.imwrite(os.path.join(args['image_dir'], str(index) + ".jpg"), frame)
        index += 1

    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break

# do a bit of cleanup
cv2.destroyAllWindows()
vs.stop()