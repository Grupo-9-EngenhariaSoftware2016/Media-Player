#include "classes.h"

// ================================================
// Metodos classe Autor

Autor::Autor()
{
    _nome = "";
    _nacionalidade = "";
    _dataNascimento = QDate::currentDate();
}

Autor::~Autor()
{

}

QString Autor::getImagem()
{
    return _imagem;
}

int Autor::setImagem(QString imagem)
{
    if(imagem == NULL)
        return -1;

    _imagem = imagem;
    return 0;
}

QString Autor::getNome()
{
    return _nome;
}

int Autor::setNome(QString nome)
{
    if(nome == NULL)
        return -1;

    _nome = nome;
    return 0;
}

QString Autor::getNacionalidade()
{
    return _nacionalidade;
}

int Autor::setNacionalidade(QString nacionalidade)
{
    if(nacionalidade == NULL)
        return -1;

    _nacionalidade = nacionalidade;
    return 0;
}

QDate Autor::getDataNascimento()
{
    return _dataNascimento;
}

int Autor::setDataNascimento(QDate data)
{
    if(data.isNull() || !data.isValid())
        return -1;

    _dataNascimento.setDate(data.year(),data.month(),data.day());
    return 0;
}

int Autor::apagar()
{
    return 0;
}

int Autor::play()
{
    return 0;
}

int Autor::criar()
{
    return 0;
}

// ================================================
// Metodos classe Musica

Musica::Musica()
{
    _nome = "";
    _diretoria = "";
    _genero = "";
    _faixa = 0;
    _autor = new QList <Autor*>;
}

Musica::~Musica()
{
    delete _autor;
}

QString Musica::getNome()
{
    return _nome;
}

int Musica::setNome(QString nome)
{
    _nome = nome;

    return 0;
}

QString Musica::getDiretoria()
{
    return _diretoria;
}

int Musica::setDiretoria(QString diretoria)
{
    _diretoria = diretoria;

    return 0;
}

int Musica::getAutor(QList <Autor*> *autores)
{
    if(!autores->isEmpty())
        return -1;

    for(QList <Autor*>::iterator current = _autor->begin();
        current != _autor->end();
        ++current)
    {
        autores->append(*current);
    }


    return 0;
}

int Musica::setAutor(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = autores->begin();
        current != autores->end();
        ++current)
    {
        _autor->append(*current);
    }

    return 0;
}

bool Musica::hasAutor(Autor *autor)
{
    return _autor->contains(autor);
}

QString Musica::getGenero()
{
    return _genero;
}

int Musica::setGenero(QString genero)
{
    _genero = genero;

    return 0;
}

int Musica::getFaixa()
{
    return _faixa;
}

int Musica::setFaixa(int faixa)
{
    _faixa = faixa;

    return 0;
}

int Musica::apagar()
{
    return 0;
}

int Musica::play()
{
    return 0;
}

// ================================================
// Metodos classe Album
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

int Album::getAutores(QList <Autor*> *autores)
{
    if(!autores->isEmpty())
        return -1;

    for(QList <Autor*>::iterator current = _autor->begin();
        current != _autor->end();
        ++current)
    {
        autores->append(*current);
    }


    return 0;
}

int Album::setAutores(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = autores->begin();
        current != autores->end();
        ++current)
    {
        _autor->append(*current);
    }

    return 0;
}

bool Album::hasAutor(Autor *autor)
{
    return _autor->contains(autor);
}

int Album::getMusicas(QList<Musica *> *musicas)
{
    if(!musicas->isEmpty())
        return -1;

    for(QList <Musica*>::iterator current = _musica->begin();
        current != _musica->end();
        ++current)
    {
        musicas->append(*current);
    }


    return 0;
}

int Album::setMusicas(QList<Musica *> *musicas)
{
    for(QList <Musica*>::iterator current = musicas->begin();
        current != musicas->end();
        ++current)
    {
        _musica->append(*current);
    }

    return 0;
}

int Album::apagar()
{
    return 0;
}

int Album::apagar(Musica *musica)
{
    return 0;
}

int Album::apagar(QList <Musica*> *musicas)
{
    return 0;
}

int Album::play()
{
    return 0;
}

int Album::adicionar(Musica *musica)
{
    if(_musica->contains(musica))
        return -1;

    _musica->append(musica);
    QList <Autor*> autores;
    musica->getAutor(&autores);

    for(int i = 0; i < autores.size(); i++)
    {
        if(!_autor->contains(autores[i]))
            _autor->append(autores[i]);
    }

    return 0;
}

int Album::criar()
{
    return 0;
}

// ================================================
// Metodos classe Playlist

Playlist::Playlist()
{
    _nome = "";
    _descricao = "";
    _musica = new QList <Musica*>;
}

Playlist::~Playlist()
{
    delete _musica;
}

QString Playlist::getNome()
{
    return _nome;
}

int Playlist::setNome(QString nome)
{
    _nome = nome;

    return 0;
}

QString Playlist::getDescricao()
{
    return _descricao;
}

int Playlist::setDescricao(QString descricao)
{
    _descricao = descricao;

    return 0;
}

int Playlist::getSize()
{
    return _musica->size();
}

int Playlist::getMusicas(QList<Musica *> *musicas)
{
    if(!musicas->isEmpty())
        return -1;

    for(QList <Musica*>::iterator current = _musica->begin();
        current != _musica->end();
        ++current)
    {
        musicas->append(*current);
    }


    return 0;
}

int Playlist::setMusicas(QList<Musica *> *musicas)
{
    for(QList <Musica*>::iterator current = musicas->begin();
        current != musicas->end();
        ++current)
    {
        _musica->append(*current);
    }

    return 0;
}

int Playlist::apagar()
{
    return 0;
}

int Playlist::play()
{
    return 0;
}

int Playlist::adicionar(Musica *musica)
{
    if(_musica->contains(musica) || musica == NULL)
        return -1;

    _musica->append(musica);
    return 0;
}

int Playlist::remover(Musica *musica)
{
    if(!_musica->contains(musica))
        return -1;

    _musica->removeOne(musica);
    return 0;
}

int Playlist::criar()
{
    return 0;
}

// ================================================
// Metodos classe Player

Player::Player()
{
    _lista = new Playlist;
    _lista->setNome("Lista de Reprodução");
    _lista->setDescricao("Lista de reprodução de player");
}

Player::~Player()
{
    delete _lista;
}

int Player::getMusicas(QList <Musica*> *lista)
{
    if(!lista->isEmpty())
        return -1;

    _lista->getMusicas(lista);

    return 0;
}

int Player::play()
{
    return 0;
}

int Player::pausa()
{
    return 0;
}

int Player::parar()
{
    return 0;
}

int Player::seguinte()
{
    return 0;
}

int Player::anterior()
{
    return 0;
}

int Player::adicionar(QList <Musica*> *musica)
{
    if(musica->isEmpty())
        return -1;

    for(QList <Musica*>::iterator current = musica->begin();
        current != musica->end();
        ++current)
    {
        _lista->adicionar(*current);
    }
    return 0;
}

int Player::remover(QList<Musica *> *musica)
{
    if(musica->isEmpty())
        return -1;

    for(QList <Musica*>::iterator current = musica->begin();
        current != musica->end();
        ++current)
    {
        _lista->remover(*current);
    }

    return 0;
}

int Player::removerTodas()
{
    QList <Musica*> *todas = new QList <Musica*>;

    _lista->getMusicas(todas);

    for(QList <Musica*>::iterator current = todas->begin();
        current != todas->end();
        ++current)
    {
        _lista->remover(*current);
    }

    delete todas;
    return 0;
}

bool Player::isEmpty()
{
    return _lista->getSize() == 0;
}