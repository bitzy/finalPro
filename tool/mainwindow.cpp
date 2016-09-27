#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include "GLOBALDEFINE.h"
#include "GLOBALCONFIG.h"
#include "GLOBALFUNC.h"
#include "attrRecognize/waysInterface.h"

#include "flagform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initial_system_dataLoad();
    ui->setupUi(this);

    QVBoxLayout *rightlayout = new QVBoxLayout;
    rightlayout->addWidget(UIcreateAttrGroup());
    rightlayout->addWidget(UIcreateConfigGroup());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(UIcreatePoseGroup());
    layout->addLayout(rightlayout, 1);

    ui->centralWidget->setLayout(layout);
    setWindowTitle(tr("Attribute Analyze Tool"));
    resize(1120, 750);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGroupBox *MainWindow::UIcreatePoseGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Pose Config Details"));

    //! [1]
    QStandardItemModel *model = new QStandardItemModel;
    QStringList tableHeader;tableHeader << "LabelName" << "DataType";
    model->setHorizontalHeaderLabels(tableHeader);
    int size = GLOBALCONFIG::inst()->getPoseCounter();
    for(int i = 0; i < size; i++) {
        model->setItem(i, 0, new QStandardItem(GLOBALCONFIG::inst()->getPoseNameByIndex(i)));
        int datatypeIdx = GLOBALCONFIG::inst()->getPTypeIdxByIdx(i);
        model->setItem(i, 1, new QStandardItem(GLOBALDEFINE::BASEDATA_NAMES[datatypeIdx]));
    }
    QTableView *table = new QTableView;
    table->setModel(model);
    //! [1]

    //! [2]
    QPushButton *exportDatatype = new QPushButton(tr("&Export System BaseData"));
    connect(exportDatatype, SIGNAL(clicked()), this, SLOT(exportBaseData()));
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(exportDatatype);
    hbox->addStretch(1);
    //! [2]

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(table);
    vbox->addLayout(hbox);
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *MainWindow::UIcreateAttrGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Attribute Detail"));

    //! [1]
    QTableView *table = new QTableView;
    model = new QStandardItemModel;
    model->setColumnCount(GLOBALCONFIG::inst()->getMaxAlternative() - 1);
    model->setVerticalHeaderLabels(GLOBALCONFIG::inst()->getAttrs());
    table->setModel(model);
    //! [1]

    //! [2]
    QString fname = QString("%1/%2").arg(GLOBALDEFINE::SYS_BULKPATH_DEFAULT).
            arg(GLOBALDEFINE::SYS_BULKPATH_DATA_FNAME);
    loadAttrTableData(fname);
    //! [2]

    //! [3]
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(table);
    groupBox->setLayout(vbox);
    int size = GLOBALCONFIG::inst()->attrKind();
    groupBox->setMaximumHeight(size*30+100);
    groupBox->setMinimumHeight(size*30+100);
    //! [3]

    return groupBox;
}

QGroupBox *MainWindow::UIcreateConfigGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Function Area"));

    //! [1]
    table2 = new QTableWidget;
    table2->setRowCount(2);
    int size = GLOBALCONFIG::inst()->attrKind();
    table2->setColumnCount(size);
    table2->setHorizontalHeaderLabels(GLOBALCONFIG::inst()->getAttrs());

    QStringList vheader;
    vheader << "CheckOption" << "Method";
    table2->setVerticalHeaderLabels(vheader);
    for(int i = 0; i < size; i++) {
        QString attrNameTmp = GLOBALCONFIG::inst()->getAttrNameByIndex(i);
        //checkbox
        QCheckBox *check = new QCheckBox;
        check->setCheckState(Qt::Checked);
        table2->setCellWidget(0, i, check);

        //functionlist:
        QComboBox *comboBox = new QComboBox;
        QStringList tmplist = ATTRWAYS::instance()->getWays(attrNameTmp);
        comboBox->addItems(tmplist);
        if(tmplist.size() == 0) check->setEnabled(false);
        table2->setCellWidget(1, i, comboBox);
    }
    table2->setMinimumHeight(100);
    table2->setMaximumHeight(100);
    //! [1]

    //! [2]
    labelBeginBtn = UIcreateButton(tr("&Label Begin"), SLOT(labelBegin()));
    labelBeginBtn->setFocusPolicy(Qt::NoFocus);
    bulkBeginBtn = UIcreateButton(tr("Bulk &Run Begin"), SLOT(bulkBegin()));
    bulkBeginBtn->setFocusPolicy(Qt::NoFocus);
    analyzeBeginBtn = UIcreateButton(tr("Data &Analyze Begin"), SLOT(analyzeBegin()));
    analyzeBeginBtn->setFocusPolicy(Qt::NoFocus);
    QLabel *lineSplit = new QLabel;
    lineSplit->setFrameShape(QFrame::HLine);
    QLabel *lineSplit1 = new QLabel;
    lineSplit1->setFrameShape(QFrame::HLine);
    LabelChoosedPath = new QLabel(GLOBALDEFINE::SYS_LABELPATH_DEFAULT);
    LabelChoosedPath->setWordWrap(true);
    BulkChoosedPath = new QLabel(GLOBALDEFINE::SYS_BULKPATH_DEFAULT);
    BulkChoosedPath->setWordWrap(true);
    analyzeChoosedFile = new QLabel;
    analyzeChoosedFile->setWordWrap(true);

    labelPathBtn = UIcreateButton(tr("&Browse..."), SLOT(browseLabelPath()));
    labelPathBtn->setFocusPolicy(Qt::NoFocus);
    bulkPathBtn = UIcreateButton(tr("&Browse..."), SLOT(browseBulkPath()));
    bulkPathBtn->setFocusPolicy(Qt::NoFocus);
    analyzePathBtn = UIcreateButton(tr("&Browse..."), SLOT(browseAnalyzeFile()));
    analyzePathBtn->setFocusPolicy(Qt::NoFocus);
    //! [2]

    //![3]
    QHBoxLayout *func1Btn = new QHBoxLayout;
    func1Btn->addWidget(labelBeginBtn);
    func1Btn->addWidget(UIcreateLabel(tr("(.xml stored in the same directory.)")));
    func1Btn->addStretch(1);

    QHBoxLayout *func2Btn = new QHBoxLayout;
    func2Btn->addWidget(bulkBeginBtn);
    func2Btn->addStretch(1);

    QHBoxLayout *func1SetPath = new QHBoxLayout;
    func1SetPath->addWidget(UIcreateLabel(tr("Label Directory:")));
    func1SetPath->addWidget(labelPathBtn);
    func1SetPath->addWidget(LabelChoosedPath);
    func1SetPath->addStretch(1);

    QHBoxLayout *func2SetPath = new QHBoxLayout;
    func2SetPath->addWidget(UIcreateLabel(tr("Bulk Directory:")));
    func2SetPath->addWidget(bulkPathBtn);
    func2SetPath->addWidget(BulkChoosedPath);
    func2SetPath->addStretch(1);
    //new add:
    QPushButton *bulkSleeveBeginBtn = UIcreateButton(tr("SleeveBulk"), SLOT(sleeveBulkBegin()));
    bulkSleeveBeginBtn->setFocusPolicy(Qt::NoFocus);
    func2SetPath->addWidget(bulkSleeveBeginBtn);

    QHBoxLayout *func3SetPath = new QHBoxLayout;
    func3SetPath->addWidget(UIcreateLabel(tr("Analyze File:")));
    func3SetPath->addWidget(analyzePathBtn);
    func3SetPath->addWidget(analyzeChoosedFile);
    func3SetPath->addStretch(1);

    QHBoxLayout *func3Btn = new QHBoxLayout;
    func3Btn->addWidget(analyzeBeginBtn);
    func3Btn->addStretch(1);
    //! [3]

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(func1SetPath);
    layout->addLayout(func1Btn);

    layout->addStretch(1);
    layout->addWidget(lineSplit);
    layout->addWidget(table2);
    layout->addLayout(func2SetPath);
    layout->addLayout(func2Btn);

    layout->addStretch(1);
    layout->addWidget(lineSplit1);
    layout->addLayout(func3SetPath);
    layout->addLayout(func3Btn);
    layout->addStretch(3);

    groupBox->setLayout(layout);
    return groupBox;
}

void MainWindow::initial_system_dataLoad()
{
    //check dir:
    GLOBALFUNC::inst()->confirmDirExist(GLOBALDEFINE::SYS_LABELPATH_DEFAULT);
    GLOBALFUNC::inst()->confirmDirExist(GLOBALDEFINE::SYS_DELPATH);
    GLOBALFUNC::inst()->confirmDirExist(GLOBALDEFINE::SYSTEST_DIR);
}

void MainWindow::loadAttrTableData(const QString& fpath)
{
    QFile file(fpath);
    bool tmpflag = false;
    if(file.exists()) {
        tmpflag = true;
        file.open(QIODevice::ReadOnly);
    }
    int size = GLOBALCONFIG::inst()->attrKind();
    for(int i = 0; i < size; i++) {
        int cols = GLOBALCONFIG::inst()->getAttrValuesByIndex(i).size();
        for(int j = 0; j < cols; j++) {
            QString showStr = GLOBALCONFIG::inst()->getAttrValue(i, j);
            if(tmpflag) {
                QString line = file.readLine(255).simplified();
                showStr.append("/");
                showStr.append(line);
            }
            QStandardItem *item = new QStandardItem(showStr);
            model->setItem(i, j, item);
        }
    }
    if(tmpflag) file.close();
}

QLabel *MainWindow::UIcreateLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
    return lbl;
}

QPushButton *MainWindow::UIcreateButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

QList<QPoint> MainWindow::getFuncNeedtoRun()
{
    QList<QPoint> res;
    int cols = table2->columnCount();
    for(int i = 0; i < cols; i++) {
        QCheckBox *check = (QCheckBox*)table2->cellWidget(0, i);
        QComboBox *combo = (QComboBox*)table2->cellWidget(1, i);
        if(check->isChecked()) {
            res.push_back(QPoint(i, combo->currentIndex()));
        }
    }
    return res;
}

void MainWindow::browseLabelPath()
{
    QString directory = QFileDialog::getExistingDirectory(
                this, tr("Find directory"),
                GLOBALDEFINE::SYS_LABELPATH_DEFAULT);
    if(!directory.isEmpty()) {
        LabelChoosedPath->setText(directory);
    }
}

void MainWindow::browseBulkPath()
{
    QString directory = QFileDialog::getExistingDirectory(
                this, tr("Find directory"), GLOBALDEFINE::SYS_BULKPATH_DEFAULT);
    if(!directory.isEmpty()) {
        BulkChoosedPath->setText(directory);

        directory = GLOBALFUNC::pathSlashAdd(directory);
        directory.append(GLOBALDEFINE::SYS_BULKPATH_DATA_FNAME);
        loadAttrTableData(directory);
    }
}

void MainWindow::browseAnalyzeFile()
{
    //browse a file:
    QString file = QFileDialog::getOpenFileName(
                this, tr("Choose a analyze file"),
                GLOBALDEFINE::SYSTEST_DIR, tr("xml (*.xml)"));
    if(!file.isEmpty()) {
        analyzeChoosedFile->setText(file);
    }
}

void MainWindow::exportBaseData()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Save system BaseData"), "",
                tr("Config File (*.config);;AllFiles (*)"));
    if(fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return ;
        }
        QTextStream out(&file);
        out << "BaseDataType:Numbers" << endl;
        int tmptypes = GLOBALDEFINE::SYSTEM_BASEDATA_TYPES;
        for(int i = 0; i < tmptypes; i++) {
            out << GLOBALDEFINE::BASEDATA_NAMES[i] << ":"
                << GLOBALDEFINE::BASEDATA_DATANUMS[i] << endl;
        }
        out.flush();
        file.close();
        QMessageBox::information(this, "Status", "Export OK", QMessageBox::Ok);
    }
}

void MainWindow::labelBegin()
{
    QString path = LabelChoosedPath->text();
    QDir t_dir(path);
    QFileInfoList tmp =  t_dir.entryInfoList(GLOBALDEFINE::imageTypefilter);
    if(tmp.count() == 0) {
        QMessageBox::information(this, "worning", tr("no .jpg file!"), QMessageBox::Ok);
        return ;
    }
    // pose data by label:
    FlagForm* flagform = new FlagForm(path, path);//store .xml in the same dir;
    flagform->setMinimumHeight(900);
    flagform->setMinimumWidth(1340);
    flagform->exec();
}

void MainWindow::bulkBegin()
{
    QString path = BulkChoosedPath->text();
    QDir t_dir(path);
    QFileInfoList tmp =  t_dir.entryInfoList(GLOBALDEFINE::imageTypefilter);
    if(tmp.count() == 0) {
        QMessageBox::information(this, "worning", tr("no .jpg file!"), QMessageBox::Ok);
        return ;
    }
    QList<QPoint> runFuncList = getFuncNeedtoRun();
    if(runFuncList.size() == 0) {
        QMessageBox::information(this, "worning", tr("no attr choosed!"), QMessageBox::Ok);
        return ;
    }

    /*
    openMidRes(false);
    //store the result in the system test directory;
    RunManager * runManager = new RunManager(
                path, SYSTEST_DIR,
                runFuncList, dataConfigFileExist,
                configFile);
    runManager->resize(800,600);
    runManager->exec();
    */
}

void MainWindow::analyzeBegin()
{
    QString analyzeFilePath = analyzeChoosedFile->text();
    if(analyzeFilePath.isEmpty()) {
        QMessageBox::information(this, "worning", tr("no file choosed!"), QMessageBox::Ok);
        return ;
    }

    /*
    Analyzer* tmpAnalyze = new Analyzer(analyzeFilePath);
    tmpAnalyze->resize(900, 600);
    tmpAnalyze->exec();
    */
}

void MainWindow::sleeveBulkBegin()
{
    QString path = BulkChoosedPath->text();
    QDir t_dir(path);
    QFileInfoList tmp = t_dir.entryInfoList(GLOBALDEFINE::imageTypefilter);
    if(tmp.count() == 0) {
        QMessageBox::information(this, "worning", tr("no .jpg file!"), QMessageBox::Ok);
        return ;
    }


    QComboBox *combo = (QComboBox*)table2->cellWidget(1, 1);
    int method = combo->currentIndex();

    /*
    openMidRes(false);
    SleeveManager* srun = new SleeveManager(path, SYSTEST_DIR, method);
    srun->resize(500, 120);
    srun->exec();*/
    cout << "ok!" << endl;
}


