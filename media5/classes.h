#ifndef CLASSES_H
#define CLASSES_H

#include <QList>
#include <QString>
#include <QDate>

using namespace std;

class Autor
{
private:
    QString _imagem;
    QString _nome;
    QString _nacionalidade;
    QDate _dataNascimento;
public:
    Autor();
    ~Autor();

    QString getImagem();
    int setImagem(QString imagem);
    QString getNome();
    int setNome(QString nome);
    QString getNacionalidade();
    int setNacionalidade(QString nacionalidade);
    QDate getDataNascimento();
    int setDataNascimento(QDate data);

    int apagar();
    int play();
    //int procurar(QString);

    int criar();

};

class Musica
{
private:
    QString _nome;
    QString _diretoria;
    QList <Autor*>* _autor;
    QString _genero;
    int _faixa;
public:
    Musica();
    ~Musica();

    QString getNome();
    int setNome(QString nome);
    QString getDiretoria();
    int setDiretoria(QString diretoria);
    int getAutor(QList <Autor*> *autores);
    int setAutor(QList <Autor*> *autores);
    bool hasAutor(Autor *autor);
    QString getGenero();
    int setGenero(QString genero);
    int getFaixa();
    int setFaixa(int faixa);

    int apagar();
    int play();
    //int procurar(QString);

};

class Album
{
private:
    QString _nome;
    QString _descricao;
    QString _diretoria;
    QString _genero;
    QString _imagem;
    QList <Autor*> *_autor;
    QList <Musica*> *_musica;
public:
	Album();
	~Album();

    QString getNome();
    int setNome(QString nome);
    QString getDescricao();
    int setDescricao(QString descricao);
    QString getDiretoria();
    int setDiretoria(QString diretoria);
    QString getGenero();
    int setGenero(QString genero);
    int getAutores(QList<Autor *> *autores);
    int setAutores(QList <Autor*> *autores);
    bool hasAutor(Autor *autor);
    QString getImagem();
    int setImagem(QString imagem);
    int getMusicas(QList <Musica*> *musicas);
    int setMusicas(QList <Musica*> *musicas);

	int apagar();
    int apagar(Musica *musica);
    int apagar(QList <Musica*> *musicas);
	int play();
    int adicionar(Musica *musica);
    //int procurar(QString);

	int criar();
	
};

class Playlist
{
private:
    QString _nome;
    QString _descricao;
    QList <Musica*>* _musica;
public:
	Playlist();
	~Playlist();

    QString getNome();
    int setNome(QString nome);
    QString getDescricao();
    int getSize();
    int setDescricao(QString descricao);
    int getMusicas(QList <Musica*> *musicas);
    int setMusicas(QList <Musica*> *musicas);

	int apagar();
	int play();
    int adicionar(Musica* musica);
    //int procurar(QString);
    int remover(Musica* musica);

	int criar();
	
};

class Player
{
private:
	Playlist* _lista;
public:
	Player();
	~Player();

    int getMusicas(QList <Musica*> *lista);
	int play();
	int pausa();
	int parar();
	int seguinte();
	int anterior();
    int adicionar(QList <Musica*> *musicas);
    int remover(QList <Musica*> *musicas);
    int removerTodas();
    bool isEmpty();
	
};

#endif // CLASSES_H
