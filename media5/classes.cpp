#include "classes.h"
#include "database.h"

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

QDate Autor::getDataAdicao()
{
    return _dataAdicao;
}

int Autor::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}

int Autor::getIdBD()
{
    return _idBD;
}

int Autor::setIdBD(int id)
{
    _idBD = id;
    return 0;
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
    Database db;

    if(db.connOpen())
    {
        _dataAdicao = QDate::currentDate();

        db.addArtist(this);

        db.connClose();
    }
    return 0;
}

bool Autor::procurar(QString procura)
{
    if(_nome.contains(procura,Qt::CaseInsensitive))
        return true;

    return false;
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

QDate Musica::getDataAdicao()
{
    return _dataAdicao;
}

int Musica::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}

int Musica::getIdBD()
{
    return _idBD;
}

int Musica::setIdBD(int id)
{
    _idBD = id;
    return 0;
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

int Musica::addAutor(Autor *autor)
{
    _autor->append(autor);
    return 0;
}

int Musica::removeAutor(Autor *autor)
{
    if(this->hasAutor(autor))
    {
        _autor->removeOne(autor);
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

int Musica::retrieveInfo(QString dir)
{
    /*
     * Método para leitura de tags id3
     * */

    return 0;
}

int Musica::apagar()
{
    /*
     * Método para remover ficheiro
     * */
    return 0;
}

int Musica::play()
{
    return 0;
}

int Musica::criar(QString diretoria)
{
    Database db;
    QString file_name, new_dir;

    if(db.connOpen())
    {
        file_name = _diretoria.right(_diretoria.size() - _diretoria.lastIndexOf("/"));
        new_dir = diretoria + file_name;

        if(QFile::copy(_diretoria, new_dir))
        {
            _diretoria = new_dir;
            _dataAdicao = QDate::currentDate();
            db.addSong(this);
        }

        db.connClose();
    }
    return 0;
}

bool Musica::procurar(QString procura)
{
    if(_nome.contains(procura,Qt::CaseInsensitive))
        return true;

    return false;
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
    _ano = 0;
    _autor = new QList <Autor*>;
    _musica = new QList <Musica*>;
}

Album::~Album()
{
    delete _autor;
    delete _musica;
}

QDate Album::getDataAdicao()
{
    return _dataAdicao;
}

int Album::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}

int Album::getIdBD()
{
    return _idBD;
}

int Album::setIdBD(int id)
{
    _idBD = id;
    return 0;
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

int Album::getAno()
{
    return _ano;
}

int Album::setAno(int ano)
{
    _ano = ano;
    return 0;
}

int Album::getAutores(QList <Autor*> *autores)
{
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
    Database db;

    for(int i = 0; i < _musica->size(); i++)
    {
        if(_musica->value(i)->apagar() == 0)
            _musica->removeAt(i);
        else
            return -1;
    }

    if(db.connOpen())
    {
        db.removeAlbum(this);

        db.connClose();
        return 0;
    }

    return -2;
}

int Album::remover(Musica *musica)
{
    if(!_musica->contains(musica))
        return -1;

    _musica->removeOne(musica);

    return 0;
}

int Album::remover(QList <Musica*> *musicas)
{
    for(int i = 0; i < musicas->size(); i++)
    {
        if(!_musica->contains(musicas->value(i)))
            return -1;

        _musica->removeOne(musicas->value(i));
    }
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

int Album::criar(QString diretoria)
{
    Database db;
    _diretoria = diretoria;
    QDir dir(_diretoria);

    if(!dir.exists())
    {
        qDebug() << "Criar " << _diretoria << "diretoria.";
        dir.mkpath(_diretoria);
    }
    else
    {
        qDebug() << _diretoria << " ja existe!";
    }

    _dataAdicao = QDate::currentDate();
    db.addAlbum(this);

    return 0;
}

bool Album::procurar(QString procura)
{
    if(_nome.contains(procura,Qt::CaseInsensitive))
        return true;

    return false;
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

QDate Playlist::getDataAdicao()
{
    return _dataAdicao;
}

int Playlist::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}

int Playlist::getIdBD()
{
    return _idBD;
}

int Playlist::setIdBD(int id)
{
    _idBD = id;
    return 0;
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
    for(QList <Musica*>::iterator current = _musica->begin();
        current != _musica->end();
        ++current)
    {
        musicas->append(*current);
    }


    return 0;
}

Musica* Playlist::getMusica(int indice)
{
    if(indice > _musica->size() || indice < 0)
        return NULL;

    return _musica->value(indice);
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

bool Playlist::procurar(QString procura)
{
    if(_nome.contains(procura,Qt::CaseInsensitive))
        return true;

    return false;
}

// ================================================
// Metodos classe Player

Player::Player()
{
    _lista = new Playlist;
    _lista->setNome("Lista de Reprodução");
    _lista->setDescricao("Lista de reprodução de player");
    _aTocar = 0;
    _aleatorio = false;
}

Player::~Player()
{
    delete _lista;
}

int Player::getMusicas(QList <Musica*> *lista)
{
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

int Player::aleatorio(bool aleatorio){
    if(_aleatorio != aleatorio)
    {
        _aleatorio = aleatorio;
    }

    if(_aleatorio){
        for(int i = 0; i < _ordem.size(); i++)
        {
            _ordem.swap(i,rand()%_ordem.size());
        }
    }else{
        std::sort(&_ordem.first(),&_ordem.last());
    }

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
        _ordem.append(_lista->getSize());
    }
    return 0;
}

int Player::adicionar(Musica *musicas)
{
    _lista->adicionar(musicas);
    _ordem.append(_lista->getSize());
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
    _ordem.clear();

    delete todas;
    return 0;
}

bool Player::isEmpty()
{
    return _lista->getSize() == 0;
}

bool Player::isRandom()
{
    return _aleatorio;
}
