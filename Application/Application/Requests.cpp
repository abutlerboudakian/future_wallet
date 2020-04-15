#include "Requests.h"

Requests::Requests(QObject * parent) : QObject(parent) {}

// -----------------------------------------------------
// Inputs                                              |
// -----------------------------------------------------


/* Function to send user inputs to run the
 * model on for an income prediction
 * @param data is a QJsonObject from all three input forms
 * @returns a vector matching Controller->metrics
 */
std::vector<double> * Requests::getPrediction(QString userId, QJsonObject Wages, QJsonObject Invest, QJsonObject Assets, int years)
{
    // Endpoint
    QUrl url(Location + QString("/submitInputs"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Body
    QJsonObject body;
    body.insert("userid", userId);
    body.insert("wages", Wages);
    body.insert("invests", Invest);
    body.insert("assets", Assets);
    body.insert("years", years);
    qDebug() << body;

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
}

/* Function to get the user's most recent inputs
 * @param userid is the userid of the currently logged in user
 * @modifies this->Data
 * @effect this->Data contains the QByteArray representation of the returned user inputs,
 *         or an error if it failed to retrieve it
 * @returns a QJsonObject of the input data
 *          or {"error":1} on error
 */
QJsonObject Requests::getInputs(QString userid)
{
    // Endpoint
    QUrl url(Location + QString("/getInputs") + QString("?userid=") + userid);

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

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->close();
    reply->deleteLater();

    if (statusCode == 200)
    {
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject res = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        return res;
    }
    else
    {
        QJsonObject res;
        res.insert("error", 1);
        return res;
    }
}

// -----------------------------------------------------
// Budgets                                             |
// -----------------------------------------------------

/* Function to add a user created budget to the database
 * @param budget is the BudgetData we plan to submit
 * @param userid is the userid we plan to add the budget to
 * @returns true if the budget was added, false otherwise
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
 * @param budgetId is the name of the budget to get
 * @param userId is the user whose budget we are getting
 * @returns BugdetData() if it failed to get the budget data
 *          or the BudgetData representation of the budget on successful retrieval
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
        for (int i = 0; i < categories.keys().size(); i++)
        {
            QString key = categories.keys()[i];
            budget->addCategory(key, categories.value(key).toDouble());
        }
    }
    return budget;
}

/* Function to get a list of budget names the user created
 * @param userId is the userid of the currently logged in user
 * @returns (true, a list of budget names) if successful
 *          (false, empty) if an error occurred.
 */
std::pair<bool, QStringList> Requests::listBudgets(QString userId)
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
    std::pair<bool, QStringList> res;
    res.first = false;
    if (statusCode == 200)
    {
        res.first = true;
        QJsonDocument jDoc = QJsonDocument::fromJson(Data);
        QJsonObject result = (jDoc.isArray() ? jDoc[0].toObject() : jDoc.object());
        QJsonArray names = result["budgets"].toArray();
        for (int i = 0; i < names.size(); i++)
        {
            res.second.push_back(names[i].toString());
        }
    }
    return res;
}


// -----------------------------------------------------
// Auth                                                |
// -----------------------------------------------------

/* Function to login a user and get their oauth
 * @param userId is the username of the person trying to log in
 * @param Password is the raw password of the person trying to log in
 * @returns true if the user credentials match, false otherwise
 */
bool Requests::login(QString userId, QString Password)
{
    // Endpoint
    QUrl url(Location + QString("/login"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //HTTP Basic authentication
    QString head = userId + ":" + QString(QCryptographicHash::hash(Password.toLocal8Bit(), QCryptographicHash::Md5).toHex());
    QByteArray data = head.toLocal8Bit().toBase64();
    QString auth = "Basic " + data;
    request.setRawHeader("Authorization", auth.toLocal8Bit());

    // Set request and callbacks
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(readData(QNetworkReply*)));

    // Send request
    QNetworkReply * reply = mgr->post(request, "{}");
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
    if (statusCode == 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Function for user to log out -- call this when app closes too
 * @param userId is the id of the currently logged in user
 */
void Requests::logout(QString userId)
{
    // Endpoint
    QUrl url(Location + QString("/predict"));

    // Header
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Body
    QJsonObject body;
    body.insert("userid", userId);

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

    reply->close();
    reply->deleteLater();

    return;
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
        for (int i = 0; i < names.size(); i++)
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
        for (int i = 0; i < names.size(); i++)
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
