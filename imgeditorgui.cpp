
#include "imgeditorgui.h"
#include "ui_imgeditorgui.h"

#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/mat.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

#include <QDebug>


using namespace cv;
using namespace  std;
const string pathResutados = "Resultados/";

ImgEditorGUI::ImgEditorGUI(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::ImgEditorGUI)
      , m_stop(true)
{
    ui->setupUi(this);
    ui->btn_CONVERT->setEnabled(false);
}

ImgEditorGUI::~ImgEditorGUI()
{
    delete ui;
}


void ImgEditorGUI::on_pushButton_clicked()
{
    destroyAllWindows();

    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(100);

    Mat imgCam;
    VideoCapture camera;
    camera=VideoCapture(ui->Camera_Idx_spinBox->value());
    std::stringstream Saveimgs;
     bool SaveImage;

    if (!camera.isOpened()){
        cout<<"!No se pudo abrir la camara!!! " << ui->Camera_Idx_spinBox->value() << "\n";
        getchar();//espera que se preione Enter para dejar seguir
    }

    camera.set(CAP_PROP_FRAME_HEIGHT,480);
    camera.set(CAP_PROP_FRAME_WIDTH,640);

    namedWindow("image",WINDOW_AUTOSIZE);
    int idxImage = 0;
    string ruta = pathResutados+ui->Name_Image_lEd->text().toStdString();
    for(;;)
    {
        camera >> imgCam;
        imshow("image", imgCam);


//        if(waitKey(30) == 13){//espera 30ms y pregunta por ENTER

            Saveimgs << ruta << "_" << idxImage << ".png";
            SaveImage = imwrite(Saveimgs.str().c_str(), imgCam, compression_params);
            Saveimgs = std::stringstream();

            idxImage++;
            if (!SaveImage){
                printf("!image %d saved failed!\n",idxImage);
            }else{
                printf("!image %d saved successful!\n",idxImage);
            }
//            if (idxImage == numImages){break;}
            if (idxImage == 500){break;}
//        }
            waitKey(ui->Delay_spinBox->value()*1000);
    }

    destroyAllWindows();
}

void ImgEditorGUI::on_destroyAllWindows()
{
    true;
}

/*
void ImgEditorGUI::on_Camera_Idx_spinBox_valueChanged(const QString &arg1)
{
    Mat imgCam;
    VideoCapture camera;
//    camera=VideoCapture(ui->Camera_Idx_spinBox->value());
    camera=VideoCapture(arg1.toInt());

    if (!camera.isOpened()){
        cout<<"!No se pudo abrir la camara!!! " << ui->Camera_Idx_spinBox->value() << "\n";
        getchar();//espera que se preione Enter para dejar seguir
    }

    camera.set(CAP_PROP_FRAME_HEIGHT,480);
    camera.set(CAP_PROP_FRAME_WIDTH,640);

    namedWindow("Camara",WINDOW_AUTOSIZE);

    for(;;)
    {
        camera >> imgCam;
        imshow("Camara", imgCam);
        waitKey(100);
//        connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(break;));
//        connect(ui->pushButton, &QPushButton::released, this, [](){break;};);
        connect(ui->pushButton, &QPushButton::released, this, &ImgEditorGUI::on_destroyAllWindows);
//        if (ui->pushButton->clicked(true) )
//        {
//            break;
//        }
    }
}
*/

void ImgEditorGUI::on_Btn_START_clicked()
{
    VideoCapture cap(ui->Camera_Idx_spinBox->value());

    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream" << endl;
    }

    // Default resolutions of the frame are obtained.The default resolutions are system dependent.
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    string nomFile= pathResutados+ui->lEd_NameVideoFrame->text().toStdString();

    VideoWriter video(nomFile+".avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, Size(frame_width,frame_height));

    cout << "Valor stop " << stop() << endl;
    while(stop()){

        Mat frame;

        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Write the frame into the file 'outcpp.avi'
        video.write(frame);

        // Display the resulting frame
    imshow( "Video", frame );

        // Press  ESC on keyboard to  exit
        char c = (char)waitKey(1);
//        if( c == 27 )
        if( stop() == 0 )
            break;
    }

    // When everything done, release the video capture and write object
    cap.release();
    video.release();

    cout << "Ended Capture video" << endl;
}

bool ImgEditorGUI::stop() const
{
    return m_stop;
}

void ImgEditorGUI::setStop(bool stop)
{
    m_stop = stop;
}

void ImgEditorGUI::on_Btn_STOP_clicked()
{
    setStop(false);
    ui->btn_CONVERT->setEnabled(true);
}

void ImgEditorGUI::on_btn_CONVERT_clicked()
{
    save_frames(pathResutados+ui->lEd_NameVideoFrame->text().toStdString()+".avi");
}

void ImgEditorGUI::save_frames(/*vector<Mat>& frames,*/ const string& outputDir)
{
    cv::VideoCapture imagen(outputDir);
    if(!imagen.isOpened())
        cout << "File video doesnt exist" << endl;
    double rate = imagen.get(CV_CAP_PROP_FPS);
    bool parar(false);
    Mat frameImage;
    namedWindow("Extracted frame");
    int delay = 1000/rate;
    int cnt=0;
    int fpvideo=ui->spBox_SampledFrames->value(); //define the frames consider per video captured
    long NoFrames = static_cast<long>(imagen.get(CV_CAP_PROP_FRAME_COUNT));
    cout << "Number of frames Ini " << NoFrames << endl;
    NoFrames=NoFrames/fpvideo;
    int avance;
    double NoFrame=1;

    //assign the max value for sample frames but this is not the correct place
    ui->spBox_SampledFrames->setMaximum((NoFrames*0.9));
    cout << "%Number of frames Ini " << NoFrames*0.9 << endl;

    if(NoFrames > 1)
    {
        while(!parar || NoFrame < NoFrames)
        {
            imagen.set(CV_CAP_PROP_POS_FRAMES, NoFrame);
            string nameFrame=pathResutados+ui->lEd_NameVideoFrame->text().toStdString()+"_"+to_string(cnt)+".png";
            if(!imagen.read(frameImage))
            {
                break;
            }
            imshow("Extracted frame", frameImage);
            imwrite(nameFrame,frameImage);

            if(waitKey(delay)>=0)
                parar=false;
            cnt++;

            avance=(cnt*100/NoFrames);
            ui->pBar_NumFrames->setValue(avance);
            cout << "Save frame  " << cnt << endl;
            NoFrame+=fpvideo;
        }

        imagen.release();

        cout << "Number of frames" << NoFrames << endl;
    }
    else
    {
        cout << "Sample Video greater than Frames" <<endl;
    }


}

void ImgEditorGUI::on_pBtn_CLOSE_clicked()
{
    destroyAllWindows();
    this->close();
}
