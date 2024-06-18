#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <unordered_map>
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_encodeButton_clicked();
    void on_decodeButton_clicked();

private:
    Ui::MainWindow *ui;

    // Define Huffman node structure
    struct node {
        char ch;
        int freq;
        node *left, *right;
    };

    // Comparator for priority queue
    struct comp {
        bool operator()(node *l, node *r) {
            return l->freq > r->freq;
        }
    };

    // Member variables
    node *root; // Root of the Huffman tree

    // Huffman coding functions
    void buildhuffmantree(const QString &text);
    void encode(node *root, QString str, std::unordered_map<char, QString> &huffmanCode);
    void decode(node *root, int &top_index, const QString &str);

    // Utility function to display Huffman codes
    void displayHuffmanCodes(std::unordered_map<char, QString> &huffmanCode);
};

#endif // MAINWINDOW_H
