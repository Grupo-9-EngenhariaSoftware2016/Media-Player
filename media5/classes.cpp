#include "classes.h"

Album::Album()
{
    _nome = "";
    _descricao = "";
    _diretoria = "";
    _genero = "";
    _imagem = "";
    _autor = new QList <Autor*>;
    _musica = new QList <Musica*>;
}

Album::~Album()
{
    while (!_autor->isEmpty())
        _autor->removeLast();

    while (!_musica->isEmpty())
        _musica->removeLast();

    delete _autor;
    delete _musica;
}

QString Album::getNome()
{
    return _nome;
}

int Album::setNome(QString nome)
{
    _nome = nome;

    return 0;
}

QString Album::getDescricao()
{
    return _descricao;
}

int Album::setDescricao(QString descricao)
{
    _descricao = descricao;

    return 0;
}

QString Album::getDiretoria()
{
    return _diretoria;
}

int Album::setDiretoria(QString diretoria)
{
    _diretoria = diretoria;

    return 0;
}

QString Album::getGenero()
{
    return _genero;
}

int Album::setGenero(QString genero)
{
    _genero = genero;

    return 0;
}

QString Album::getImagem()
{
    return _imagem;
}

int Album::setImagem(QString imagem)
{
    _imagem = imagem;

    return 0;
}

