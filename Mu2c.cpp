#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

class MusicClient : public QWidget {
public:
    MusicClient(QWidget *parent = nullptr) : QWidget(parent) {
        QListWidget *listWidget = new QListWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(listWidget);
        setLayout(layout);

        readConfig("config.conf", listWidget);
    }

    void readConfig(const QString &filename, QListWidget *listWidget) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error opening config file";
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed(); // Trim whitespace
            if (!line.isEmpty()) {
                listWidget->addItem(line);
            }
        }

        file.close();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MusicClient client;
    client.setWindowTitle("Music Client");
    client.setGeometry(100, 100, 400, 300);
    client.show();

    return app.exec();
}
