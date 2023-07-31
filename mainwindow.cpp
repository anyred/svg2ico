#include "mainwindow.h"
#include "./ui_mainwindow.h"

#pragma pack(push)
#pragma pack(1)

struct ico_header
{
    short reserved;  // 0
    short type;      // 1:ico 2:cursor
    short count;     // images count;
};

struct ico_entry
{
    uint8_t     width;
    uint8_t     height;
    uint8_t     color_count;
    uint8_t     reserved;
    uint16_t    planes;
    uint16_t    bit_count;
    uint32_t    bytes_in_res;
    uint32_t    image_offset;
};

#pragma pack(pop)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_count = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_select_in_path_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->in_path->setText(dir);
    ui->out_path->setText(dir+"/out");
}

void MainWindow::TraversalDir(QString path, QString prefix)
{
    if(!path.isEmpty())
    {
        QDir dir(path);
        if(!dir.exists())
        {
            return;
        }

        QVector<QString> file;
        QStringList filters;
//        filters << QString("*.svg");
//        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::Dirs);
        QFileInfoList list = dir.entryInfoList();
        int i = 0;
        int item = 0;
        int index = 0;
        QString file_path;
        do
        {
            QFileInfo fileinfo = list.at(i++);
            if(fileinfo.fileName() == "." || fileinfo.fileName() == "..")
            {
                continue;
            }
            else if(fileinfo.isDir())
            {
                // 建立 N+1个文件夹
                TraversalDir(path+"/"+fileinfo.fileName(),fileinfo.fileName());
            }
            else
            {
                if(fileinfo.suffix() == "svg")
                {
                    index = file.size();
                    QString name_full;
                    if(prefix == "")
                    {
                         name_full = fileinfo.baseName();
                    }
                    else
                    {
                         name_full = prefix + "_" + fileinfo.baseName();
                    }

                    // 枚举PX,创建好ico后,压入file
                    for (int item = 0; item < m_px_count; item++) {
                        file_path = ui->out_path->text()+"/single/"+ReviseString(name_full.toUpper())+"_"+m_px.at(item)+".ico";
                        QFile ico_file(file_path);
                        ico_file.open(QIODevice::NewOnly | QIODevice::ReadWrite);
                        CreateIcon(fileinfo.filePath(),m_px.at(item).toInt(),m_px.at(item).toInt(),&ico_file);
                        ico_file.flush();
                        ico_file.close();
                        // 添加文件
                        file.push_back(file_path);
                    }
                    // 将file中的各种PX组合为一个ICO
                    // 这里的图标用来建立DLL 自适应的icon
                    QFile group(ui->out_path->text()+"/group/IDI_"+ReviseString(name_full.toUpper())+".ico");
                    group.open(QIODevice::NewOnly | QIODevice::ReadWrite);
                    CreateIconGroup(&group, file, index);
                    // 建立n+1个文件
                    AddData(fileinfo, prefix, item++);
                }
                else{
                    continue;
                }
            }
        }
        while(i < list.size());

        QVector<QString>().swap(file);
    }
}

void MainWindow::AddData(QFileInfo& file, QString& prefix, int index)
{
    QString name(ReviseString(prefix+file.baseName()));

    ui->out_data->insertRow(m_count);
    // 文件名
    ui->out_data->setItem(m_count, 0, new QTableWidgetItem(QString(prefix+file.baseName())));
    // 修正后的名字
    ui->out_data->setItem(m_count, 1, new QTableWidgetItem(name.toUpper()));
    // 资源文件ID
    ui->out_data->setItem(m_count, 2, new QTableWidgetItem("IDI_" + name.toUpper()+"{"+ui->pixel->text()+"}"));
    // DLL资源 eg:IDI_NAME_{}


    QDir dir(file.dir());
    name = ReviseString(dir.dirName());
    // 得到Index
    ui->out_data->setItem(m_count, 3, new QTableWidgetItem("IDI_" + name.toUpper() + "{" + ui->pixel->text() + "}"+":"+QString::number(index)));
    // 填充路径
    ui->out_data->setItem(m_count, 4, new QTableWidgetItem(file.filePath()));
    m_count++;
}


void MainWindow::on_make_clicked()
{
    if(ui->pixel->text().length() > 0)
    {
        QString pixel =  ui->pixel->text();
        if(pixel.at(pixel.size() -1) == ',')
        {
            ui->pixel->setText(QString(pixel.data(),pixel.size()-1));
        }
        // 删除文件夹

        m_px = ui->pixel->text().split(',');
        m_px_count = m_px.size();
        IsEnable(false);
        // 建立文件夹
        QDir out(ui->out_path->text());
        out.removeRecursively();
        out.mkdir(ui->out_path->text());
        out.mkdir(ui->out_path->text()+"/single");
        out.mkdir(ui->out_path->text()+"/group");
        TraversalDir(ui->in_path->text());
        ui->out_data->resizeColumnsToContents();
        ui->out_data->resizeRowsToContents();
        IsEnable(true);
    }
}


void MainWindow::on_select_out_path_clicked()
{
    ui->out_path->setText(QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::on_build_clicked()
{
    IsEnable(false);
    BuildRc();
    IsEnable(true);
}


void MainWindow::IsEnable(bool var)
{
    ui->in_path->setEnabled(var);
    ui->out_path->setEnabled(var);
    ui->pixel->setEnabled(var);
    ui->make->setEnabled(var);
    ui->select_in_path->setEnabled(var);
    ui->select_out_path->setEnabled(var);
    ui->out_data->setEnabled(var);
    ui->build->setEnabled(var);
}


void MainWindow::Clear()
{
    if(m_count != 0)
    {
        ui->out_data->clearContents();
        m_count = 0;
    }
}

void MainWindow::CreateIcon(QString path, int w, int h, QString out)
{
    QImage image(w,h,QImage::Format_RGB32);
    image.fill(Qt::color0);
    image.scaled(w,h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QSvgRenderer svg(path);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    svg.render(&painter);
    image.save(out,"ico");
}

// https://en.wikipedia.org/wiki/ICO_(file_format)
// https://www.codeproject.com/Articles/6171/Access-multiple-icons-in-a-single-icon-file
void MainWindow::CreateIconGroup(QFile* out, QVector<QString>& ico, int index)
{
    ico_header header;
    out->seek(0);
    header.reserved = 0;
    header.type = 1;
    header.count = ico.size() - index;
    // 写入文件头
    out->write((char*)&header, sizeof(header));
    QByteArray entry_list;
    QByteArray ico_data;
    uint32_t data_offset = sizeof(ico_header) + sizeof(ico_entry);
    uint32_t offset = sizeof(ico_header) +  (header.count * sizeof(ico_entry));
    ico_entry entry;
    // 写入文件的entry域
    for(int i = index; i < ico.size(); i++)
    {
        // 得到单个文件
        QFile file(ico.at(i));
        file.open(QIODevice::ReadOnly);
        // 移动文件指针到entry区域
        file.seek(sizeof(ico_header));
        // 得到ico_entry数据
        file.read((char*)&entry, sizeof(entry));
        entry.image_offset = offset;
        entry_list.append((char*)&entry,sizeof(entry));
        file.seek(data_offset);
        int data_size = file.size()-data_offset;
        ico_data.append(file.read(data_size));
        offset += data_size;

    }
    out->write(entry_list);
    out->write(ico_data);
}

void MainWindow::CreateIcon(QString path, int w, int h, QFile* file)
{
    QImage image(w,h,QImage::Format_RGB32);
    image.fill(Qt::color0);
    image.scaled(w,h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QSvgRenderer svg(path);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    svg.render(&painter);
    image.save(file,"ico");
}


void MainWindow::BuildRc()
{
    QString name = QDir(ui->in_path->text()).dirName();
    QFile header(ui->out_path->text()+"/"+name+".h");
    QFile rc(ui->out_path->text()+"/"+name+".rc");
    header.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text);
    rc.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text);
    rc.write("#include \"");
    rc.write(name.toStdString().c_str(),name.toStdString().length());
    rc.write(".h\"\n");
    rc.write("#define APSTUDIO_READONLY_SYMBOLS\n#include \"afxres.h\"\n#undef APSTUDIO_READONLY_SYMBOLS\n#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\n#ifdef _WIN32\nLANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US\n#pragma code_page(1252)\n#endif\n#ifdef APSTUDIO_INVOKED\n1 TEXTINCLUDE DISCARDABLE\nBEGIN\n");
    rc.write("    \"");
    rc.write(name.toStdString().c_str(),name.toStdString().length());
    rc.write(".h\\0\"\nEND\n");
    rc.write("2 TEXTINCLUDE DISCARDABLE \nBEGIN\n");
    rc.write("    \"#include \"\"afxres.h\"\"\\r\\n\"\n    \"\\0\"\nEND\n");
    rc.write("3 TEXTINCLUDE DISCARDABLE \nBEGIN\n");
    rc.write("    \"\\r\\n\"\n    \"\\0\"\nEND\n#endif\n\n\n");
    QDir dir(ui->out_path->text()+"/group");
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    QString mapping;
    mapping = "\n\n#include<string>\n#include<map>\n\nstd::map<std::string, int> index={\n";
    QString temp;
    char data[100];
    for (int i = 0; i < list.size(); ++i) {
        //IDI_ICON1               ICON    DISCARDABLE     "icon1.ico"
        QFileInfo fileinfo = list.at(i);
        int n = sprintf(data, "%s               ICON    DISCARDABLE     \"%s\"\n",
                        fileinfo.baseName().toStdString().c_str(),QString(dir.dirName()+"\\"+fileinfo.fileName()).toStdString().c_str());
        rc.write(data, n);
        n = sprintf(data, "#define %s            %d\n", fileinfo.baseName().toStdString().c_str(),101+i);
        header.write(data,n);
        sprintf(data,"{\"%s\",%s}%c\n",fileinfo.baseName().right(fileinfo.baseName().size()-4).toLower().toStdString().c_str(),fileinfo.baseName().toStdString().c_str(),(i != list.size()-1) ? ',' : ' ');
        mapping.append(data);
    }
    mapping += "};\n\n";

    header.write(mapping.toStdString().c_str());
    header.write("\n\n#ifdef APSTUDIO_INVOKED\n#ifndef APSTUDIO_READONLY_SYMBOLS\n#define _APS_NEXT_RESOURCE_VALUE        102\n#define _APS_NEXT_COMMAND_VALUE         40001\n#define _APS_NEXT_CONTROL_VALUE         1000\n#define _APS_NEXT_SYMED_VALUE           101\n#endif\n#endif");
    rc.write("#endif \n#ifndef APSTUDIO_INVOKED\n\n#endif\n");
}


QString MainWindow::ReviseString(QString name)
{
    QString revise = name;
    for (QChar*it = revise.begin(); it != revise.end(); ++it) {
        if(!it->isLetterOrNumber())
        {
            *it = QChar('_');
        }
    }
    return revise;
}
