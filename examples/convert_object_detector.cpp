#include <dlib/svm_threaded.h>
#include <dlib/string.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/cmd_line_parser.h>

#include <iostream>
#include <fstream>

using namespace dlib;
using namespace std;

int main(int argc, char** argv)
{  
    try
    {
        command_line_parser parser;
        parser.add_option("h","Display this help message.");
        parser.add_option("u", "What upsample was used?", 1);
        parser.parse(argc, argv);

        if (parser.option("h"))
        {
            cout << "Usage: convert_object_detector [options] <input file> <output file>\n";
            parser.print_options(); 
                                       
            return EXIT_SUCCESS;
        }

        // Get the upsample option from the user but use 2 if it wasn't given.
        const unsigned long upsample_amount = get_option(parser, "u", 2);

        if (parser.number_of_arguments() != 2)
        {
            cout << "You must supply two arguments" << endl;
            cout << "\nTry the -h option for more information." << endl;
            return EXIT_FAILURE;
        }

        // load a previously trained object detector and try it out on some data
        ifstream fin(parser[0], ios::binary);
        if (!fin)
        {
            cout << "Can't find a trained object detector file " << parser[0] << endl;
            cout << "\nTry the -h option for more information." << endl;
            return EXIT_FAILURE;

        }
        typedef scan_fhog_pyramid<pyramid_down<6> > image_scanner_type; 
        object_detector<image_scanner_type> detector;
        deserialize(detector, fin);

        // From serialize_object_detector.h, the python way of saving
        std::ofstream fout(parser[1], std::ios::binary);
        int version = 1;
        serialize(detector, fout);
        serialize(version, fout);
        serialize(upsample_amount, fout);
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
        cout << "\nTry the -h option for more information." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}