#include <QCoreApplication>
#include <frame_reader.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Frame_reader reader;
    reader.open("/home/dima/Documents/1.bin");
    SpecFrame frame;
    reader.load(&frame);
    reader.close();
    return a.exec();
}
