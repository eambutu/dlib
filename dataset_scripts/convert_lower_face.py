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
ap.add_argument("--out-xml", required=True,
    help="path to output xml file")
args = vars(ap.parse_args())

xml_tree = ET.parse(os.path.join(args['base_dir'], args['xml_file']))
root = xml_tree.getroot()
for child in root:
    if child.tag == 'images':
        for image_segment in child:
            to_remove = []
            for box_segment in image_segment:
                x = int(box_segment.attrib['left'])
                y = int(box_segment.attrib['top'])
                w = int(box_segment.attrib['width'])
                h = int(box_segment.attrib['height'])
                if w * h / 2 <= 711:  # The limit for how small a box can be
                    to_remove.append(box_segment)
                else:
                    box_segment.attrib['top'] = str(y + int(h/2))
                    box_segment.attrib['height'] = str(int(h / 2))
            for box in to_remove:
                image_segment.remove(box)

xml_tree.write(os.path.join(args['base_dir'], args['out_xml']))