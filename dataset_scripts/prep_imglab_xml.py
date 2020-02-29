import argparse
import cv2
import time
import os
import xml.etree.ElementTree as ET

ap = argparse.ArgumentParser()
ap.add_argument("--images-dir", required=True,
    help="base directory of data")
ap.add_argument("--xml-file", required=True,
	help="path to xml")
args = vars(ap.parse_args())

xml_tree = ET.parse(args['xml_file'])
root = xml_tree.getroot()
for child in root:
    if child.tag == 'images':
        for image_segment in child:
            image_segment.attrib['file'] = os.path.join(args['images_dir'], image_segment.attrib['file'])

xml_tree.write(args['xml_file'])