#include "plotter.h"

int scatter_main(QWidget * widget);

Plotter::Plotter()
{
    str = "";
    json_log = R"(
      {
        "happy": true,
        "pi": 3.141
      }
    )"_json;
}


//******************************************************************************
//  TODO:Logging System for Clustering in json
//  Change a serial log list into structured json format
//
// {
//  graph:{
//      point_number: int
//      position:[n][x,y,z]
//      cost:[combination(n,3)]
//      cost':[combination(n,3)]
//  }
//  clustering process:{
//      round-1:{
//          move-1{
//          }
//          ...
//          move-n{
//          }
//          best-move{
//          }
//      }
//      ...
//      round-n:{
//      ...
//      }
//    }
// }
//******************************************************************************

Plotter* Plotter::log(std::string d_type, void *pointer, int lenth, std::string name, float diff){
    if(name.compare("")!=0){
        str += "**************************";
        str += name;
        str += "**************************\n";
        json_log[""] = name;
    }

    //******************************************************************************
    //    Print Pi and the diffrence between phi(Pi) and a given min value
    //******************************************************************************

    if(d_type.compare("diff") == 0){

        str += "After Move: ";
        int *pi = (int*)pointer;
        str += "{" ;
        for(int i = 0; i < lenth-1; i++){
            str += std::to_string(pi[i]);
            str += ", " ;
        }
        str += std::to_string(pi[lenth-1]);
        str += "} Diffrence ---->" ;
        str += std::to_string(diff);

        if(name.compare("Best Move Of The Sequence") == 0){}
        if(name.compare("Better Move Found") == 0){}
        if(name.compare("Final Selected Partition") == 0){}
    }

    //***************************************
    //   Default
    //***************************************
    else{
        str += "diff Print UNSUCCESSFUL: print option ";
        str += name;
        str += "not found\n";
    }
    str += ("\n");

    if(name.compare("")!=0){
        str += "\n\n " ;
    }
    return this;
}

Plotter* Plotter::log(std::string d_type, void *pointer, int lenth, std::string name, int second_dim){
    if(name.compare("")!=0){                                            //Seprater if needed
        str += "**************************";
        str += name;
        str += "**************************\n";
    }
    //***************************************
    //    Print A Set Of Int Array
    //***************************************
    if(d_type.compare("int") == 0){
        int* p = (int*)pointer;
        str += name;
        str += ": \n{";
        for(int i = 0; i < lenth-1; i++){
            str += std::to_string(p[i]);
            str += ", " ;
        }
        str += std::to_string(p[lenth-1]);
        str += "}" ;
    }

    //***************************************
    //    Print A Set Of Float Array
    //***************************************

    else if(d_type.compare("float") == 0){
        float *theta = (float*)pointer;
        str += name;
        str += ": \n";
        for(int i = 0; i < lenth-1; i++){
            str += std::to_string(theta[i]);
            str += " << " ;
        }
        str += std::to_string(theta[lenth-1]);
    }

    //***************************************
    //    Print A 2D Float Array for x
    //***************************************

    else if(d_type.compare("float2D") == 0){
        float **x = (float**)pointer;
        str += name;
        str += ": \n";

        for(int i = 0; i < lenth; i++){
            str += std::to_string(i);
            str += (" : ");
            for(int j = 0; j < second_dim -1; j++){
                str += std::to_string(x[i][j]);
                str += (" << ");
            }
            str += std::to_string(x[i][second_dim-1]);
            str += ("\n");
        }
    }

    //**********************************************
    //    Print c and c', test order of c if needed
    //**********************************************


    else if(d_type.compare("c") == 0){
        float **x = (float**)pointer;
        str += name;
        str += ": \n";

        for(int i = 0; i < lenth; i++){
            str += std::to_string(i);
            str += (" : ");
            for(int j = 0; j < second_dim -1; j++){
                str += std::to_string(x[i][j]);
                str += (" << ");
            }
            str += std::to_string(x[i][second_dim-1]);
            str += ("\n");
        }
    }




    //***************************************
    //   Default
    //***************************************
    else{
        str += "Print UNSUCCESSFUL: print option ";
        str += name;
        str += "not found\n";
    }
    str += ("\n");

    return this;
}

Plotter* Plotter::print_to_cout(){
    std::cout<<str;
    std::cout<<std::flush;
    str = "";
    return this;
}

//**********************************************************************************************
//   QApplication::exec  Must be called from the main thread
//
//   Asynchronization version:
//   1. Prepare the clustering data first.
//   2. Call the Qt by Plotter, pass data to it and visualization.
//**********************************************************************************************

Plotter* Plotter::print_to_qtTextField(){
    int argc = 1;
    char *argv[1] = {};
    QApplication a(argc, argv);
    w = new MainWindow;
    w->log(str);
    scatter_main(new QWidget());
    w->show();
    a.exec();
    str = "";
    return this;
}

void ScatterDataModifier::addData(json json)
{
    // Configure the axes according to the data

    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(json["point_number"]);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    for(int i = 0; i < json["point_number"]; i++){
        ptrToDataArray->setPosition(QVector3D(json["position"][i][0],
                                              json["position"][i][1],
                                              json["position"][i][2]));
    }


    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}








































//**********************************************************************************************
//   TODO:  Refactor the hard-copy code into a new folder
//      These code are copyed from a Qt Example project
//**********************************************************************************************



int scatter_main(QWidget * widget)
{
    //! [0]QApplication app(argc, argv);
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);
    //! [0]

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    //! [1]
    //QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    //! [1]

    widget->setWindowTitle(QStringLiteral("Cube Clustering"));

    //! [4]
    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(widget);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(widget);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);

    QComboBox *itemStyleList = new QComboBox(widget);
    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
    itemStyleList->setCurrentIndex(0);

    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *itemCountButton = new QPushButton(widget);
    itemCountButton->setText(QStringLiteral("Toggle item count"));

    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(widget);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(widget);
    fontList->setCurrentFont(QFont("Arial"));
    //! [4]

    //! [5]
    vLayout->addWidget(labelButton, 0, Qt::AlignTop);
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
    vLayout->addWidget(backgroundCheckBox);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
    vLayout->addWidget(itemStyleList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    vLayout->addWidget(shadowQuality);
    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    vLayout->addWidget(fontList, 1, Qt::AlignTop);
    //! [5]

    //! [2]
    ScatterDataModifier *modifier = new ScatterDataModifier(graph);
    //! [2]

    //! [6]
    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &ScatterDataModifier::changePresetCamera);
    QObject::connect(labelButton, &QPushButton::clicked, modifier,
                     &ScatterDataModifier::changeLabelStyle);
    QObject::connect(itemCountButton, &QPushButton::clicked, modifier,
                     &ScatterDataModifier::toggleItemCount);

    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setBackgroundEnabled);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setGridEnabled);
    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setSmoothDots);

    QObject::connect(modifier, &ScatterDataModifier::backgroundEnabledChanged,
                     backgroundCheckBox, &QCheckBox::setChecked);
    QObject::connect(modifier, &ScatterDataModifier::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);
    QObject::connect(itemStyleList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeStyle(int)));

    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeTheme(int)));

    QObject::connect(shadowQuality, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeShadowQuality(int)));

    QObject::connect(modifier, &ScatterDataModifier::shadowQualityChanged, shadowQuality,
                     &QComboBox::setCurrentIndex);
    QObject::connect(graph, &Q3DScatter::shadowQualityChanged, modifier,
                     &ScatterDataModifier::shadowQualityUpdatedByVisual);

    QObject::connect(fontList, &QFontComboBox::currentFontChanged, modifier,
                     &ScatterDataModifier::changeFont);

    QObject::connect(modifier, &ScatterDataModifier::fontChanged, fontList,
                     &QFontComboBox::setCurrentFont);
    //! [6]

    //! [3]
    //widget->show();
    return 0;
    //! [3] app.exec();
}

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QAbstract3DSeries::MeshSphere),
      m_smooth(true),
      m_itemCount(lowerNumberOfItems),
      m_curveDivider(lowerCurveDivider)
{
    //! [0]
    m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
    //! [0]

    //! [2]
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
    //! [2]

    //! [3]
    addData();
    //! [3]
}

//#define RANDOM_SCATTER // Uncomment this to switch to random scatter
ScatterDataModifier::~ScatterDataModifier()
{
    delete m_graph;
}

void ScatterDataModifier::addData()
{
    // Configure the axes according to the data
    //! [4]
    m_graph->axisX()->setTitle("X");
    m_graph->axisY()->setTitle("Y");
    m_graph->axisZ()->setTitle("Z");
    //! [4]

    //! [5]
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(m_itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    //! [5]

#ifdef RANDOM_SCATTER
    for (int i = 0; i < m_itemCount; i++) {
        ptrToDataArray->setPosition(randVector());
        ptrToDataArray++;
    }
#else
    //! [6]
    float limit = qSqrt(m_itemCount) / 2.0f;
    for (float i = -limit; i < limit; i++) {
        for (float j = -limit; j < limit; j++) {
            ptrToDataArray->setPosition(QVector3D(i + 0.5f,
                                                  qCos(qDegreesToRadians((i * j) / m_curveDivider)),
                                                  j + 0.5f));
            ptrToDataArray++;
        }
    }
    //! [6]
#endif

    //! [7]
    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
    //! [7]
}

//! [8]
void ScatterDataModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (m_graph->seriesList().size())
            m_graph->seriesList().at(0)->setMesh(m_style);
    }
}

void ScatterDataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = m_graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void ScatterDataModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void ScatterDataModifier::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void ScatterDataModifier::changeLabelStyle()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void ScatterDataModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    m_graph->activeTheme()->setFont(newFont);
}

void ScatterDataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterDataModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void ScatterDataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void ScatterDataModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled((bool)enabled);
}
//! [8]

void ScatterDataModifier::toggleItemCount()
{
    if (m_itemCount == numberOfItems) {
        m_itemCount = lowerNumberOfItems;
        m_curveDivider = lowerCurveDivider;
    } else {
        m_itemCount = numberOfItems;
        m_curveDivider = curveDivider;
    }
    m_graph->seriesList().at(0)->dataProxy()->resetArray(0);
    addData();
}

QVector3D ScatterDataModifier::randVector()
{
    return QVector3D(
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f);
}
