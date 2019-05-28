#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

int faceLandmarksDetection()
{
	try
	{	//mở camera
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			cerr << "Unable to connect to camera" << endl;
			return 1;
		}

		image_window win;

		//Load face_detection ( nhận diện khuôn mặt)
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;//file shape_predictor_68 này là file data default mẫu mặt mặc định của dlib
		//dlib's default face landmarking model

		// Liên tục bắt frame rồi xử lí khi cửa sổ chưa đóng
		while (!win.is_closed())
		{
			// Grab a frame
			cv::Mat temp;
			if (!cap.read(temp))
			{
				break;
			}
			

			//dữ liệu lấy ra từ Mat temp ->chuyển vào cimg() của dlib()-> hàm cimg chỉ tồn tại và còn xử lí
			// khi temp còn lưu trữ giá trị ,tức là khi cửa sổ Win còn mở và còn bắt Frame
			cv_image<bgr_pixel> cimg(temp);

			// Detect faces 
			std::vector<rectangle> faces = detector(cimg);
			// Find the pose of each face.
			std::vector<full_object_detection> shapes;
			for (unsigned long i = 0; i < faces.size(); ++i)
				shapes.push_back(pose_model(cimg, faces[i]));

			// Display it all on the screen
			win.clear_overlay();
			win.set_image(cimg);
			win.add_overlay(render_face_detections(shapes));
		}
	}
	catch (serialization_error& e)
	{

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
}

