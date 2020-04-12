#include "Requests.h"

Requests::Requests(QObject * parent) : QObject(parent) {}

/* Function to send user inputs to run the
 * model on for an income prediction
 */
void Requests::getPrediction(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    QJsonDocument result = QJsonDocument::fromJson(Data);
    qDebug() << result;
    qDebug() << result[0]; // Since it was in array form
    QJsonObject jsonResult = result[0].toObject(); // since we need the object itself
    qDebug() << jsonResult; // shows that it is the object
    qDebug() << jsonResult["BudgetName"].toString(); // to get the budget name
    //std::cout<<((QJsonDocument::fromJson(QString(Data).toUtf8()))["BudgetName"]).toString().toStdString()<<std::endl;
}

// Slot function to set this->Data to the repsonse data
void Requests::readData(QNetworkReply *reply)
{
   Data = reply->readAll();
   reply->close();
   reply->deleteLater();
}

/* Function to get the user's most recent inputs
 */
void Requests::getInputs(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function to add a user created budget to the database
 */
void Requests::addBudget(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function to get budget information on a user specific budget
 */
void Requests::loadBudget(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function to get a list of budget names the user created
 */
void Requests::listBudgets(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function to login a user and get their oauth
 */
void Requests::login(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function for user to log out -- call this when app closes too
 */
void Requests::logout(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

/* Function to get a list of all industries for the combo box
 */
void Requests::getIndustries(...)
{
    // Endpoint
    QUrl url("http://127.0.0.1:5000/predict");

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("test", 5);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}
