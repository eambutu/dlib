#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------
inline std::vector<image_window::overlay_line> custom_render_face_detections (
        const std::vector<full_object_detection>& dets,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<image_window::overlay_line> lines;
        for (unsigned long i = 0; i < dets.size(); ++i)
        {
            DLIB_CASSERT(dets[i].num_parts() == 11,
                "\t std::vector<image_window::overlay_line> custom_render_face_detections()"
                << "\n\t You have to give a 11 point"
                << "\n\t dets["<<i<<"].num_parts():  " << dets[i].num_parts() 
            );

            const full_object_detection& d = dets[i];

            lines.push_back(image_window::overlay_line(d.part(0), d.part(1), color));
            lines.push_back(image_window::overlay_line(d.part(1), d.part(3), color));
            lines.push_back(image_window::overlay_line(d.part(3), d.part(4), color));
            lines.push_back(image_window::overlay_line(d.part(4), d.part(5), color));
            lines.push_back(image_window::overlay_line(d.part(5), d.part(6), color));
            lines.push_back(image_window::overlay_line(d.part(6), d.part(7), color));
            lines.push_back(image_window::overlay_line(d.part(7), d.part(8), color));
            lines.push_back(image_window::overlay_line(d.part(8), d.part(0), color));
            lines.push_back(image_window::overlay_line(d.part(9), d.part(10), color));
            lines.push_back(image_window::overlay_line(d.part(10), d.part(2), color));
        }
        return lines;
    }

// ----------------------------------------------------------------------------------------

inline std::vector<image_window::overlay_line> custom_render_face_detections (
        const full_object_detection& det,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<full_object_detection> dets;
        dets.push_back(det);
        return custom_render_face_detections(dets, color);
    }

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc < 3)
        {
            cout << "Call this program like this:" << endl;
            cout << "./test_custom_face [detector file] [shape predictor file] [imagepath1] [imagepath2] ..." << endl;
            return 0;
        }

        typedef scan_fhog_pyramid<pyramid_down<6> > image_scanner_type; 
        object_detector<image_scanner_type> detector;
        deserialize(argv[1]) >> detector;

        shape_predictor sp;
        deserialize(argv[2]) >> sp;

        image_window win, win_faces;
        // Loop over all the images provided on the command line.
        for (int i = 3; i < argc; ++i)
        {
            cout << "processing image " << argv[i] << endl;
            array2d<rgb_pixel> img;
            load_image(img, argv[i]);
            // Make the image larger so we can detect small faces.
            pyramid_up(img);

            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces in the image.
            std::vector<rectangle> dets = detector(img);
            cout << "Number of faces detected: " << dets.size() << endl;

            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.
            std::vector<full_object_detection> shapes;
            for (unsigned long j = 0; j < dets.size(); ++j)
            {
                full_object_detection shape = sp(img, dets[j]);
                cout << "number of parts: "<< shape.num_parts() << endl;
                cout << "pixel position of first part:  " << shape.part(0) << endl;
                cout << "pixel position of second part: " << shape.part(1) << endl;
                // You get the idea, you can get all the face part locations if
                // you want them.  Here we just store them in shapes so we can
                // put them on the screen.
                shapes.push_back(shape);
            }

            // Now let's view our face poses on the screen.
            win.clear_overlay();
            win.set_image(img);
            win.add_overlay(custom_render_face_detections(shapes));

            // We can also extract copies of each face that are cropped, rotated upright,
            // and scaled to a standard size as shown here:
            /*
            dlib::array<array2d<rgb_pixel> > face_chips;
            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            win_faces.set_image(tile_images(face_chips));
            */

            cout << "Hit enter to process the next image..." << endl;
            cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}