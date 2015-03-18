#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

#define SET_PREPOINT_POS
//#define USE_PREPOINT_POS

int point_range = 15;
Scalar point_color = Scalar(255,0,255);
vector<Point2i> DFPoint;

void printDFPoint(){
	
	int ssize = DFPoint.size();
	for( int i=0; i<ssize; i++ ){
		cout << "Point: " << i << "; " << DFPoint[i] << endl;
	}

}

void drawDFPoint(Mat &imgDrawFinger){

	int ssize = DFPoint.size();
	for( int i=0; i<ssize; i++ ){
		cv::circle(imgDrawFinger, DFPoint[i], point_range, point_color, -1, CV_AA);
	}

}

#ifdef SET_PREPOINT_POS
// return the point clicked if excited
int findDFPoint(Point2i ppos, int range){
	
	// DFPoint is empty
	int ssize = DFPoint.size();
	if( ssize < 1 ){
		return -3;
	}

	for( int i=0; i<ssize; i++ ){
		if ( ppos.x < DFPoint[i].x + range && ppos.x > DFPoint[i].x - range && ppos.y < DFPoint[i].y + range && ppos.y > DFPoint[i].y - range){
			return i;
		}
	}
	return -2;

}

void addDFPoint(Point2i ppos){

	DFPoint.push_back(ppos);

}

void delDFPoint(Point2i ppos){
	
	int pid = findDFPoint(ppos, point_range);
	//cout << pid << endl;
	if (pid >= 0){
		DFPoint.erase(DFPoint.begin() + pid);
	}

}

void storeDFPoint(){

	ofstream myfile;
	myfile.open ("DFPoint.txt");
	int ssize = DFPoint.size();
	for( int i=0; i<ssize; i++ ){
		myfile << DFPoint[i] << endl;
	}
	myfile.close();
}

// handle the mouse events
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN ){
		addDFPoint(Point2i(x, y));
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_RBUTTONDOWN ){
		delDFPoint(Point2i(x, y));
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN ){
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if ( event == EVENT_MOUSEMOVE ){
        // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    }
}

#endif

#ifdef USE_PREPOINT_POS

void loadDFPoint(){
	
	string line;
	ifstream myfile ("DFPoint.txt");
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			
			// delete "[", ",", "]"
			char chars[] = "[,]";
			for (unsigned int i = 0; i < strlen(chars); ++i){
				line.erase (std::remove(line.begin(), line.end(), chars[i]), line.end());
			}

			// string to vector<string>
			std::stringstream ss(line);
			std::istream_iterator<std::string> begin(ss);
			std::istream_iterator<std::string> end;
			std::vector<std::string> vstrings(begin, end);
			/*for (int i=0; i<vstrings.size(); i++){
				cout << vstrings[i] << " ";	
			}
			cout << endl;*/
			if (vstrings.size() == 2){
				int tempx = std::atoi(vstrings[0].c_str());
				int tempy = std::atoi(vstrings[1].c_str());
				DFPoint.push_back(Point2i(tempx, tempy));
			}else{
				cout << "ERROR FILE INPUT!!!" << endl;
			}
	    }

	    myfile.close();
	}

}

#endif

int main()
{
	namedWindow("FaceWindow", 1);
	 
#ifdef SET_PREPOINT_POS
	//set the callback function for any mouse event
	setMouseCallback("FaceWindow", CallBackFunc, NULL);
#endif

	// the face image
	Mat imgDrawFace = imread("face2.jpg");

	while(1){

		// the draw info
		Mat imgDrawFinger = Mat::zeros(imgDrawFace.rows,imgDrawFace.cols,CV_8UC3);

		drawDFPoint(imgDrawFinger);
		imgDrawFinger = imgDrawFace - imgDrawFinger;

		imshow("FaceWindow",imgDrawFinger);
		
		char key = cv::waitKey(10);

		if (key == '1'){
			printDFPoint();
		}
#ifdef SET_PREPOINT_POS
		if (key == '2'){
			storeDFPoint();
			cout << "Stored!" << endl;
		}
#endif
#ifdef USE_PREPOINT_POS
		if (key == '3'){
			loadDFPoint();
		}
#endif
	}
}