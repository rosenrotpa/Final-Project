//#include <ros/ros.h>
//#include <sensor_msgs/Image.h>
//#include <cv_bridge/cv_bridge.h>

using namespace cv;

void imageCallback(const sensor_msgs::Image::ConstPtr& image_msg){
  ROS_INFO("i've got a image");
  cv_bridge::CvImagePtr asd = cv_bridge::toCvCopy(image_msg,);

 Mat src, src_gray;

 /// Read the image
 src = imread( argv[1], 1 );

  if( !src.data )
    { return -1; }

 /// Convert it to gray
 cvtColor( src, src_gray, CV_BGR2GRAY );

 /// Reduce the noise so we avoid false circle detection
 GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

 vector<Vec3f> circles;

 /// Apply the Hough Transform to find the circles
 HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

 /// Draw the circles detected
 for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
  }

}

int main(int argc,char** argv){
  ros::init(argc, argv, "circle_detector");
  ros::NodeHandle nh;

  ros::Subscriber image_raw = nh.subscribe("point", 10, &imageCallback);

  ros::spin();

  return 0;
}
