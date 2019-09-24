#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

#include "sortserver.h"
#define DEFAULT_PORT 1234


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QCoreApplication::setApplicationName("sortserver");
	QCoreApplication::setApplicationVersion("1.0");
	QCommandLineParser parser;

	QCommandLineOption portOption("p");
	QStringList argsList;
	parser.addOption(portOption);

	parser.process(a);
	int port = DEFAULT_PORT;

	if(parser.isSet(portOption)) {
		bool parseok = false;
		if(argc == 3) {
			if(QString(argv[1]) == "-p") {
				port = QString(argv[2]).toInt(&parseok);
				if(!parseok)
					port = DEFAULT_PORT;
			}	
		}
	}
	SortServer server(port);

	return a.exec();
}
