#include "plotter.h"

int scatter_main(QWidget * widget);

Plotter::Plotter()
{
    ENABLE_TEACE = false;
    str = "";
    json_log = R"(
      {
        "point_number": 4,
        "number_of_planes": 4
      }
    )"_json;
}


//******************************************************************************
//  TODO:Logging System for Clustering in json
//  Change a serial log list into structured json format
//
// {
//  graph:{
//      "point_number": int,
//      "number_of_planes": 4
//      "position":[n][x,y,z],
//      "cost":[combination(n,3)],
//      "cost'":[combination(n,3)]
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
Plotter* Plotter::enable(){
    ENABLE_TEACE = true;
    return this;
}
Plotter* Plotter::disable(){
    ENABLE_TEACE = false;
    return this;
}
Plotter* Plotter::log(std::string d_type, void *pointer, int lenth, std::string name, float diff){
    if(name.compare("")!=0 && ENABLE_TEACE){
        str += "**************************";
        str += name;
        str += "**************************\n";
    }

    //******************************************************************************
    //    Print Pi and the diffrence between phi(Pi) and a given min value
    //******************************************************************************

    if(d_type.compare("diff") == 0 && ENABLE_TEACE){

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

        if(name.compare("Best Move Of The Sequence") == 0){
            std::cout<<str<<std::flush;
            str = "";
        }
        if(name.compare("Better Move Found") == 0){

            std::cout<<str<<std::flush;
            str = "";
        }
        if(name.compare("Final Selected Partition") == 0){}
    }

    //***************************************
    //   Default
    //***************************************
    else if(ENABLE_TEACE){
        str += "diff Print UNSUCCESSFUL: print option ";
        str += name;
        str += "not found\n";
    }
    if(ENABLE_TEACE){
        str += ("\n");
        if(name.compare("")!=0 && ENABLE_TEACE){
            str += "\n" ;
        }
    }
    return this;
}

Plotter* Plotter::log(std::string d_type, void *pointer, int lenth, std::string name, int second_dim){
    if(name.compare("")!=0 && ENABLE_TEACE){                                            //Seprater if needed
        str += "**************************";
        str += name;
        str += "**************************\n";
    }
    //***************************************
    //    Print A Set Of Int Array
    //***************************************
    if(d_type.compare("int") == 0 && ENABLE_TEACE){
        int* p = (int*)pointer;
        str += name;
        str += ": \n{";
        for(int i = 0; i < lenth-1; i++){
            str += std::to_string(p[i]);
            str += ", " ;
        }
        str += std::to_string(p[lenth-1]);
        str += "}" ;        
        str += ("\n");
    }

    //***************************************
    //    Print A Set Of Float Array
    //***************************************

    else if(d_type.compare("float") == 0 && ENABLE_TEACE){
        float *theta = (float*)pointer;
        str += name;
        str += ": \n";
        for(int i = 0; i < lenth-1; i++){
            str += std::to_string(theta[i]);
            str += " << " ;
        }
        str += std::to_string(theta[lenth-1]);        
        str += ("\n");
    }

    //***************************************
    //    Print A 2D Float Array for x
    //***************************************

    else if(d_type.compare("float2D") == 0 && ENABLE_TEACE){
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


    else if(d_type.compare("c") == 0 && ENABLE_TEACE){
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
    //    Print points using scatter
    //**********************************************

    else if(d_type.compare("points") == 0){
        json_log["point_number"] = lenth;
        float *x = (float*)pointer;
        for(int i = 0; i < lenth; i++){
            json_log["position"][i][0]=x[i*3];
            json_log["position"][i][1]=x[i*3+1];
            json_log["position"][i][2]=x[i*3+2];
        }
    }
    else if(d_type.compare("pi") == 0){
        int *x = (int*)pointer;
        for(int i = 0; i < lenth; i++){
            json_log["pi"][i]=x[i];
        }
    }
    else if(name.compare("gt") == 0){
        int *x = (int*)pointer;
        for(int i = 0; i < lenth; i++){
            json_log["gt"][i]=x[i];
        }
    }

    //***************************************
    //   Default
    //***************************************
    else if(ENABLE_TEACE){
        str += "Print UNSUCCESSFUL: print option ";
        str += name;
        str += "not found\n";
    }

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
    scatter_main(new QWidget());
    a.exec();
    return this;
}

void ScatterDataModifier::addData(json json)
{
    // Configure the axes according to the data
    int POINTS = json["point_number"];
    int PLANES = json["number_of_planes"];
    int dots = POINTS / PLANES;
    std::vector<Qt::GlobalColor> color(20);
    //TODO: Add color list for more planes;
    color[0] = Qt::white;
    color[1] = Qt::green;
    color[2] = Qt::red;
    color[3] = Qt::blue;


//**********************************************************************************************
//   Add Ground Truth Data
//**********************************************************************************************

    if(json.contains("gt")){
        std::vector<QScatterDataArray*> gtResult(PLANES);
        std::vector<QScatterDataItem*> ptrToGroundTruth(PLANES);
        std::vector<int> count(PLANES);
        for (int i = 0; i<PLANES; i++){
            gtResult[i] = new QScatterDataArray;
            gtResult[i]->resize(POINTS);
            ptrToGroundTruth[i] = &gtResult[i]->first();
        }
        for(int i = 0; i < POINTS; i++){

            ptrToGroundTruth[json["gt"][i]]->setPosition(
                        QVector3D(json["position"][i][0],
                                  json["position"][i][1],
                                  json["position"][i][2]));
            ptrToGroundTruth[json["gt"][i]]++;
            count[json["gt"][i]]++;
        }
        for (int i = 0; i<PLANES; i++){
            gtResult[i] -> resize(count[i]);
            m_graph->seriesList().at(i)->dataProxy()->resetArray(gtResult[i]);
            m_graph->seriesList().at(i)->setBaseColor(color[i]);
        }
    }else{
        QScatterDataArray *dataArray;
        QScatterDataItem *ptrToDataArray;
        for(int i = 0; i < POINTS; i++){
            if (i % dots == 0){
                dataArray = new QScatterDataArray;
                dataArray->resize(dots);
                m_graph->seriesList().at(i/dots)->dataProxy()->resetArray(dataArray);
                m_graph->seriesList().at(i/dots)->setBaseColor(color[i/dots]);
                ptrToDataArray = &dataArray->first();
            }
            ptrToDataArray->setPosition(QVector3D(json["position"][i][0],
                                                  json["position"][i][1],
                                                  json["position"][i][2]));
            ptrToDataArray++;
        }
    }




//**********************************************************************************************
//   Add Data from json["position"] and color from json["pi"]
//**********************************************************************************************
    std::vector<QScatterDataArray*> dataArrayResult(PLANES);
    std::vector<QScatterDataItem*> ptrToPartition(PLANES);
    std::vector<int> count(PLANES);
    for (int i = 0; i<PLANES; i++){
        dataArrayResult[i] = new QScatterDataArray;
        dataArrayResult[i]->resize(POINTS);
        ptrToPartition[i] = &dataArrayResult[i]->first();
    }
    for(int i = 0; i < POINTS; i++){

        ptrToPartition[json["pi"][i]]->setPosition(
                    QVector3D(json["position"][i][0],
                              json["position"][i][1],
                              json["position"][i][2]));
        ptrToPartition[json["pi"][i]]++;
        count[json["pi"][i]]++;
    }
    for (int i = 0; i<PLANES; i++){
        dataArrayResult[i] -> resize(count[i]);
        m_graph->seriesList().at(i+PLANES)->dataProxy()->resetArray(dataArrayResult[i]);
        m_graph->seriesList().at(i+PLANES)->setBaseColor(color[i]);
        m_graph->seriesList().at(i)->setVisible(false);
        m_graph->seriesList().at(i+PLANES)->setVisible(true);

    }
}





//**********************************************************************************************
//   TODO:  Refactor the hard-copy code into a new folder
//      These code are copyed from a Qt Example project  Copyright (C) 2016 The Qt Company Ltd. Contact: https://www.qt.io/licensing/
//**********************************************************************************************



int Plotter::scatter_main(QWidget * widget)
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

    BiStateBtn *itemCountButton = new BiStateBtn(widget);
    itemCountButton->setText(QStringLiteral("Show Ground Truth"));


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

    QObject::connect(itemCountButton, &BiStateBtn::clicked,itemCountButton,
                    &BiStateBtn::switchState);

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
    modifier->addData(json_log);
    widget->show();
    return 0;
    //! [3] app.exec();
}

BiStateBtn::BiStateBtn(QWidget* parent):
    QPushButton(parent),
    SHOW_GT(false){
}
void BiStateBtn::switchState(){
    if (SHOW_GT){
        this->setText("Show Ground Truth");
        SHOW_GT = false;
    }else{
        this->setText("Show Cluster Result");
        SHOW_GT = true;
    }
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
    QScatterDataProxy *proxy2 = new QScatterDataProxy;
    QScatter3DSeries *series2 = new QScatter3DSeries(proxy2);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    QScatterDataProxy *proxy3 = new QScatterDataProxy;
    QScatter3DSeries *series3 = new QScatter3DSeries(proxy3);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    QScatterDataProxy *proxy4 = new QScatterDataProxy;
    QScatter3DSeries *series4 = new QScatter3DSeries(proxy4);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
    m_graph->addSeries(series2);
    m_graph->addSeries(series3);
    m_graph->addSeries(series4);
    m_graph->addSeries(new QScatter3DSeries(new QScatterDataProxy));
    m_graph->addSeries(new QScatter3DSeries(new QScatterDataProxy));
    m_graph->addSeries(new QScatter3DSeries(new QScatterDataProxy));
    m_graph->addSeries(new QScatter3DSeries(new QScatterDataProxy));

    //! [2]

    //! [3]
    //addData();
    //! [3]
}

//#define RANDOM_SCATTER // Uncomment this to switch to random scatter
ScatterDataModifier::~ScatterDataModifier()
{
    delete m_graph;
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
        m_graph->seriesList().at(0)->setVisible(false);
        m_graph->seriesList().at(1)->setVisible(false);
        m_graph->seriesList().at(2)->setVisible(false);
        m_graph->seriesList().at(3)->setVisible(false);
        m_graph->seriesList().at(4)->setVisible(true);
        m_graph->seriesList().at(5)->setVisible(true);
        m_graph->seriesList().at(6)->setVisible(true);
        m_graph->seriesList().at(7)->setVisible(true);
    } else {
        m_itemCount = numberOfItems;
        m_curveDivider = curveDivider;
        m_graph->seriesList().at(0)->setVisible(true);
        m_graph->seriesList().at(1)->setVisible(true);
        m_graph->seriesList().at(2)->setVisible(true);
        m_graph->seriesList().at(3)->setVisible(true);
        m_graph->seriesList().at(4)->setVisible(false);
        m_graph->seriesList().at(5)->setVisible(false);
        m_graph->seriesList().at(6)->setVisible(false);
        m_graph->seriesList().at(7)->setVisible(false);
    }
}
