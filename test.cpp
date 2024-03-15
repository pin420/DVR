#include <iostream>
#include <opencv2/opencv.hpp>

#include "opencv2/videoio/legacy/constants_c.h"
#include "opencv2/imgproc/types_c.h"

int main() {
    cv::VideoCapture capture(0);

    double fps = capture.get( CV_CAP_PROP_FPS );

    if (!capture.isOpened()) {
        std::cerr << "Error opening camera" << std::endl;
        return -1;
    }

    cv::Size frameSize(
        (int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT)
    );

    cv::VideoWriter writer;
    writer.open("outputVideo.mp4", CV_FOURCC('H', '2', '6', '4'), fps, frameSize);

    if (!writer.isOpened()) {
        std::cerr << "Error opening output video file" << std::endl;
        return -1;
    }

    cv::Mat frame;

    while (true) {
        capture >> frame;

        if (frame.empty()) {
            break;
        }

        writer.write(frame);
        cv::imshow("Camera", frame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    capture.release();
    writer.release();

    cv::destroyAllWindows();

    return 0;
}
