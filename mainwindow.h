#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStatusBar>
#include <QComboBox>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QListWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStringList>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QSvgRenderer>
#include <QPainter>
#include <QImage>
#include <QMessageBox>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void TraversalDir(QString path, QString prefix = NULL);

    void AddData(QFileInfo& file, QString& prefix, int index);

    void IsEnable(bool var = true);

    void Clear();

    void WriteIcon(QString idi, QStringList& px ,QString out);

    void WriteIcon_PX(QString name, QStringList&px ,QString out);

    void CreateIcon(QString path, int w, int h, QString out);

    void CreateIcon(QString path, int w, int h, QFile* out);

    void CreateIconGroup(QString path, QStringList&px, QString out);

    void CreateIconGroup(QFile* out, QVector<QString>& data, int index);

    void BuildRc();

    QString ReviseString(QString name);



private slots:
    void on_select_in_path_clicked();

    void on_make_clicked();

    void on_select_out_path_clicked();

    void on_build_clicked();

private:
    Ui::MainWindow              *ui;
    int                         m_count;           // file count
    QVector<QString>            m_index;           // name && index;
    QVector<QString>            m_index_alone;     // name && index;
    QVector<QString>            m_index_px;        // name && index;
    QVector<QString>            m_index_px_alone;  // name && index;
    QStringList                 m_px;
    int                         m_px_count;
    QMap<QString, QArrayData>   m_data;
    QFile                       m_all;
    QString                     m_all_index;
    //SVGIconEngine               m_svg;
};
#endif // MAINWINDOW_H


/*
 *
 *   *Res.h [* is folder name, if is multiple folders, name is the top-level folder name]
 *
 *std::map<string, DWORD> icon_index =
 *{
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *  {name, IDI_NAME},
 *};
 *
 *  图标带
 *  IDI_NAME  i 表示下标.
 *  name      表示这个图标的名字,为了避免名字重复, 根目录为原名,子目录下的名字的带文件夹的名称
 *  引用的时候,带上通过名称得到下标,然后通过下标来获取图片的名称.
 *
 * The name is generated from the file name, the suffix is removed, and special characters in the file name are replaced by underscores
 * 生成RC文件
 * 将图标放进去, vc工具生成 资源 dll
 * 引用方式和资源dll相同.
 * 命令行,生成dll资源与头文件.
 * IDS_图标的名字    字符串
 * IDI_图标的名字    图标
 * 生成的RC文件
 * .h文件固定
 *
 *
*/
