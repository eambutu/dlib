import argparse
import cv2
import time
import os
import xml.etree.ElementTree as ET

ap = argparse.ArgumentParser()
ap.add_argument("--base-dir", required=True,
    help="base directory of data")
ap.add_argument("--xml-file", required=True,
	help="path to xml, after base")
args = vars(ap.parse_args())

xml_tree = ET.parse(os.path.join(args['base_dir'], args['xml_file']))
root = xml_tree.getroot()
for child in root:
    if child.tag == 'images':
        for image_segment in child:
            im = cv2.imread(os.path.join(args['base_dir'], image_segment.attrib['file']))
            for box_segment in image_segment:
                x = int(box_segment.attrib['left'])
                y = int(box_segment.attrib['top'])
                w = int(box_segment.attrib['width'])
                h = int(box_segment.attrib['height'])
                im = cv2.rectangle(im, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.imshow("Frame", im)
            cv2.waitKey()