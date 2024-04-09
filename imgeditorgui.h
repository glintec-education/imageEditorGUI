#ifndef IMGEDITORGUI_H
#define IMGEDITORGUI_H

#include <QMainWindow>

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

QT_BEGIN_NAMESPACE
namespace Ui { class ImgEditorGUI; }
QT_END_NAMESPACE

class ImgEditorGUI : public QMainWindow
{
    Q_OBJECT

public:
    ImgEditorGUI(QWidget *parent = nullptr);
    ~ImgEditorGUI();

    bool stop() const;
    void setStop(bool stop);
    void save_frames(/*std::vector<cv::Mat>& frames,*/ const std::string& outputDir);
    void extract_frames(const std::string &videoFilePath,std::vector<cv::Mat>& frames);

private slots:
    void on_pushButton_clicked();
    void on_destroyAllWindows();
//    void on_Camera_Idx_spinBox_valueChanged(const QString &arg1);

    void on_Btn_START_clicked();

    void on_Btn_STOP_clicked();

    void on_btn_CONVERT_clicked();

    void on_pBtn_CLOSE_clicked();

private:
    Ui::ImgEditorGUI *ui;
    bool m_stop;
};
#endif // IMGEDITORGUI_H
