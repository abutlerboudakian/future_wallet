#include "Requests.h"

Requests::Requests(QObject * parent) : QObject(parent) {}

// -----------------------------------------------------
// Inputs                                              |
// -----------------------------------------------------


/* Function to send user inputs to run the
 * model on for an income prediction
 * @param
 * @returns a vector matching Controller->metrics
 */
std::vector<double> * Requests::getPrediction(...)
{
    // Endpoint
    QUrl url(Location + QString("/submitInputs"));

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
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    // Assume it is already an object
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    std::vector<double> * metric = new std::vector<double>;
    if (statusCode == 200)
    { // Success
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        metric->push_back(result["wages"].toDouble());
        metric->push_back(result["invests"].toDouble());
        metric->push_back(result["assets"].toDouble());
        metric->push_back(result["years"].toInt());
    }
    return metric;
    //qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    /*QJsonDocument result = QJsonDocument::fromJson(Data);
    qDebug() << result;
    qDebug() << result[0]; // Since it was in array form
    QJsonObject jsonResult = result[0].toObject(); // since we need the object itself
    qDebug() << jsonResult; // shows that it is the object
    qDebug() << jsonResult["BudgetName"].toString(); // to get the budget name*/
    //std::cout<<((QJsonDocument::fromJson(QString(Data).toUtf8()))["BudgetName"]).toString().toStdString()<<std::endl;
}

/* Function to get the user's most recent inputs
 */
void Requests::getInputs(...)
{
    // Endpoint
    QUrl url(Location + QString("/getInputs"));

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
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
}

// -----------------------------------------------------
// Budgets                                             |
// -----------------------------------------------------

/* Function to add a user created budget to the database
 * @param budget is the BudgetData we plan to submit
 * @param userid is the userid we plan to add the budget to
 */
bool Requests::addBudget(BudgetData * budget, QString userid)
{
    // Endpoint
    QUrl url(Location + QString("/submitBudget"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body, categories;
    body.insert("userid", userid);
    //body.insert("name", budget->getName());
    const ChartMap * cats = budget->getBudgetChartMap();
    for (ChartMap::const_iterator i = cats->begin(); i != cats->end(); i++)
    {   // Add each category
        categories.insert(QString::fromStdString(std::string(i->first)), i->second);
    }
    body.insert("categories", categories);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    if (statusCode == 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Function to get budget information on a user specific budget
 */
BudgetData * Requests::loadBudget(QString budgetId, QString userId)
{
    // Endpoint
    QUrl url(Location + QString("/getBudget?userid=") + userId + QString("&budgetid=") + budgetId);

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->get(request);
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();

    BudgetData * budget = new BudgetData;
    qDebug() << "made it here";

    if (statusCode == 200)
    {
        qDebug() << "made it eeeeeeee";
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        // budget->setName(result["name"].toString().toStdString());
        QJsonObject categories = result["categories"].toObject();
        for (unsigned int i = 0; i < categories.keys().size(); i++)
        {
            QString key = categories.keys()[i];
            budget->addCategory(key, categories.value(key).toDouble());
        }
    }
    return budget;
}

/* Function to get a list of budget names the user created
 * @returns a list of budget names
 */
QStringList Requests::listBudgets(QString userId)
{
    // Endpoint
    QUrl url(Location + QString("/getAllBudgets?userid=") + userId);

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->get(request);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    QStringList BudgetNames;
    if (statusCode == 200)
    {
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        QJsonArray names = result["budgets"].toArray();
        for (unsigned int i = 0; i < names.size(); i++)
        {
            BudgetNames.push_back(names[i].toString());
        }
    }
    qDebug() << BudgetNames;
    return BudgetNames;
}


// -----------------------------------------------------
// Auth                                                |
// -----------------------------------------------------

/* Function to login a user and get their oauth
 */
bool Requests::login(...)
{
    // Endpoint
    QUrl url(Location + QString("/login"));

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
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    if (statusCode != 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Function for user to log out -- call this when app closes too
 */
void Requests::logout(...)
{
    // Endpoint
    QUrl url(Location + QString("/predict"));

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
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    std::cout<<"Damn it works"<<std::endl;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
}

/* Function to POST registration information to the database
 * @param userId is the user email
 * @param password is the user password
 * @returns true if the user was created
 *          false otherwise
 */
bool Requests::Register(QString UserId, QString Password)
{
    // Endpoint
    QUrl url(Location + QString("/register"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("userid", UserId);
    body.insert("password", QString(QCryptographicHash::hash(Password.toLocal8Bit(), QCryptographicHash::Md5).toHex()));

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    if (statusCode == 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Function used to update the account information
 * @param OriginalUserId is the original user id/email
 * @param userId is the new email (keep it "" if it is not to be updated)
 * @param Password is the new password (keep it "" if it is not to be updated)
 * @modifies database user credentials
 * @effects database user crecentials
 */
bool Requests::UpdateUserInfo(QString OriginalUserId, QString userId, QString Password)
{
    // Endpoint
    QUrl url(Location + QString("/update"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("userid", OriginalUserId);
    QJsonObject updates;
    if (userId != QString(""))
    {
        updates.insert("userid", userId);
    }
    if (Password != QString(""))
    {
        updates.insert("password", QString(QCryptographicHash::hash(Password.toLocal8Bit(), QCryptographicHash::Md5).toHex()));
    }
    body.insert("updates", updates);

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->post(request, QJsonDocument(body).toJson());
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    if (statusCode == 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// -----------------------------------------------------
// Aux                                              |
// -----------------------------------------------------

/* Function to get a list of all industries for the combo box
 * @returns a QStringList of industry codes
 */
QStringList Requests::getIndustries()
{
    // Endpoint
    QUrl url(Location + QString("/getIndustries"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->get(request);
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    QStringList Industries;
    if (statusCode == 200)
    {
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        QJsonArray names = result["industries"].toArray();
        for (unsigned int i = 0; i < names.size(); i++)
        {
            Industries.push_back(names[i].toString());
        }
    }
    return Industries;
}

/* Function to get a list of all stock tickers for the combo box
 * @returns a QStringList of stock tickers
 */
QStringList Requests::getStocks()
{
    // Endpoint
    QUrl url(Location + QString("/getTickers"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->get(request);
    //QJsonDocument jsonData = QJsonDocument::fromJson(response_data);

    // Make it syncrhonous so we can process it here
    QEventLoop MakeSync;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),&MakeSync,SLOT(quit()));
    MakeSync.exec();

    // Do stuff
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();
    QStringList Tickers;
    if (statusCode == 200)
    {
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        QJsonArray names = result["tickers"].toArray();
        for (unsigned int i = 0; i < names.size(); i++)
        {
            Tickers.push_back(names[i].toString());
        }
    }
    return Tickers;
}


// Slot function to set this->Data to the repsonse data
void Requests::readData(QNetworkReply *reply)
{
   Data = reply->readAll();
}
