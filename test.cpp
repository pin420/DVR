#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/videoio/legacy/constants_c.h"
#include "opencv2/imgproc/types_c.h"
#include "thread_pool.h"

cv::VideoWriter writer;

int function(cv::Mat& frame) {
    if (writer.isOpened()) {
        writer.write(frame);
    } else {
        std::cerr << "VideoWriter is not opened" << std::endl;
    }

    return 0;
}

int main() {
    cv::VideoCapture capture(0);

    double fps = capture.get(cv::CAP_PROP_FPS);

    if (!capture.isOpened()) {
        std::cerr << "Error opening camera" << std::endl;
        return -1;
    }

    cv::Size frameSize(
        (int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT)
    );

    writer.open("outputVideo.mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);

    if (!writer.isOpened()) {
        std::cerr << "Error opening output video file" << std::endl;
        return -1;
    }

    cv::Mat frame;

    thread_pool threadPool(4);

    while (true) {
        capture >> frame;

        if (frame.empty()) {
            break;
        }

        threadPool.add_task(function, frame);

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

