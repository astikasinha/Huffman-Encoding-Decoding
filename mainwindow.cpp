#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize root to nullptr
    root = nullptr;

    // Connect signals and slots
    connect(ui->Encode, SIGNAL(clicked()), this, SLOT(on_encodeButton_clicked()));
    connect(ui->Decode, SIGNAL(clicked()), this, SLOT(on_decodeButton_clicked()));
}

MainWindow::~MainWindow()
{
    // Clean up memory
    delete root;
    delete ui;
}

void MainWindow::on_encodeButton_clicked()
{
    QString text = ui->Encodestring->toPlainText();

    // Clean up existing Huffman tree
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }

    buildhuffmantree(text);
}

void MainWindow::on_decodeButton_clicked()
{
    QString encodedString = ui->Encodestring->toPlainText();
    int top_index = -1;
    decode(root, top_index, encodedString);
}

void MainWindow::buildhuffmantree(const QString &text)
{
    // Clear existing Huffman tree
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }

    // Count frequency of each character
    std::unordered_map<char, int> freq;
    for (QChar ch : text) {
        freq[ch.toLatin1()]++;
    }

    // Priority queue to store Huffman tree nodes
    std::priority_queue<node *, std::vector<node *>, comp> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto pair : freq) {
        pq.push(new node{pair.first, pair.second, nullptr, nullptr});
    }

    // Build Huffman tree
    while (pq.size() > 1) {
        node *left = pq.top(); pq.pop();
        node *right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new node{'\0', sum, left, right});
    }

    root = pq.top(); // Assign the root of Huffman tree

    // Generate Huffman codes and display
    std::unordered_map<char, QString> huffmanCode;
    encode(root, "", huffmanCode);

    // Display Huffman codes
    displayHuffmanCodes(huffmanCode);
}

void MainWindow::encode(node *root, QString str, std::unordered_map<char, QString> &huffmanCode)
{
    if (root == nullptr) return;

    // Leaf node found, store the Huffman code
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str.isEmpty() ? "1" : str; // Handle single character case
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void MainWindow::decode(node *root, int &top_index, const QString &str)
{
    if (root == nullptr) return;

    // Leaf node found, append decoded character
    if (!root->left && !root->right) {
        ui->Displaydecode->insertPlainText(QString(root->ch));
        return;
    }

    top_index++;

    if (top_index < str.length()) {
        if (str[top_index] == '0') {
            decode(root->left, top_index, str);
        } else {
            decode(root->right, top_index, str);
        }
    }
}

void MainWindow::displayHuffmanCodes(std::unordered_map<char, QString> &huffmanCode)
{
    ui->Displayencode->clear();

    for (auto pair : huffmanCode) {
        ui->Displayencode->insertPlainText(QString(pair.first) + " : " + pair.second + "\n");
    }
}
