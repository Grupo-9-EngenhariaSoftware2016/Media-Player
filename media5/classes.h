#ifndef CLASSES_H
#define CLASSES_H

#include <QList>
#include <QString>

using namespace std;

class Autor
{
private:
    QString _nome;
    QString _nacionalidade;
    time_t _dataNascimento;
public:
    Autor();
    ~Autor();

    QString getNome();
    QString getNacionalidade();
    time_t getDataNascimento();
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
    QString getDiretoria();
    int getAutor(QList <Autor*>&);
    QString getGenero();
    int getFaixa();
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
    QList <Autor*>* _autor;
    QList <Musica*>* _musica;
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
    int getAutores(QList <Autor*>& autores);
    int setAutores(QList <Autor*>& autores);
    QString getImagem();
    int setImagem(QString imagem);
    int getMusicas(QList <Musica*>& musicas);
    int setMusicas(QList <Musica*>& musicas);

	int apagar();
	int play();
    int adicionar(Musica& musica);
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
    QString getDescricao();
    int getMusica(QList <Musica*>&);
	int apagar();
	int play();
	int adicionar();
    //int procurar(QString);
	int remover();

	int criar();
	
};

class Player
{
private:
	Playlist* _lista;
public:
	Player();
	~Player();

	int play();
	int pausa();
	int parar();
	int seguinte();
	int anterior();
    int adicionar(Musica* musica);
    int remover(int indice);
    int removerTodas();
    bool isEmpty();
	
};

#endif // CLASSES_H
