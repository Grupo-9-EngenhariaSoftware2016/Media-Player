#include "classes.h"
#include "database.h"

bool removeDir(const QString &dirName); //https://john.nachtimwald.com/2010/06/08/qt-remove-directory-and-its-contents/

// ================================================
// Metodos classe Autor

Autor:: Autor()
{
    _nome = "";
    _nacionalidade = "";
    _dataNascimento = QDate::currentDate();
}
Autor::~Autor()
{

}

QDate   Autor::getDataAdicao()
{
    return _dataAdicao;
}
int     Autor::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}
int     Autor::getIdBD()
{
    return _idBD;
}
int     Autor::setIdBD(int id)
{
    _idBD = id;
    return 0;
}
QString Autor::getImagem()
{
    return _imagem;
}
int     Autor::setImagem(QString imagem)
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
int     Autor::setNome(QString nome)
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
int     Autor::setNacionalidade(QString nacionalidade)
{
    if(nacionalidade == NULL)
        return -1;

    _nacionalidade = nacionalidade;
    return 0;
}
QDate   Autor::getDataNascimento()
{
    return _dataNascimento;
}
int     Autor::setDataNascimento(QDate data)
{
    if(data.isNull() || !data.isValid())
        return -1;

    _dataNascimento.setDate(data.year(),data.month(),data.day());
    return 0;
}
int     Autor::apagar()
{
    //Remover ficheiro de Imagem do Autor
    QFile img(this->getImagem());
    if(img.remove())
    {
        qDebug() << "REMOVE: Imagem do Autor " << this->getIdBD();
    }

    //Apagar informação na BD
    Database db;
    db.connOpen();
    db.removeArtist(this);
    db.connClose();

    /*
     * Falta Apagar as musicas pertencentes ao album
     */
    return 0;
}
int     Autor::play()
{
    /*
     * Obsoleto? isto está a ser feito em mainwindow
     * */
    return 0;
}
int     Autor::criar()
{
    QString autor_path = QDir::currentPath() +"/debug/autor";
    if(!QDir(autor_path).exists())
    {
        qDebug() << "Criar " << autor_path << "diretoria.";
        QDir().mkpath(autor_path);
    }

    Database db;
    if(db.connOpen())
    {
        _dataAdicao = QDate::currentDate();

        QString oldImgDir = this->getImagem();

         qDebug()<<"InIT DB";
        if(db.addArtist(this))
        {
            qDebug()<<"Autor Adicionado à DB";
            //qDebug()<<"\n oldDir:"<< oldImgDir << "\n NEwDir:"<< this->getImagem();
            //QFile::copy(oldImgDir,this->getImagem()); //copiar Imagem para a pasta do album

            if(this->getImagem() != NULL)
            {

                QImage img(oldImgDir);
                QPixmap pixmap;
                pixmap = pixmap.fromImage(img.scaled(250,250,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QFileInfo fi(this->getImagem());

                QFile file(this->getImagem());

                file.open(QIODevice::WriteOnly);
                if(fi.suffix()== "jpg")
                {
                    pixmap.save(&file, "jpg",100);
                }

                if(fi.suffix()== "png")
                {
                    pixmap.save(&file, "png",100);
                }

                file.close();
            }
        }

        db.connClose();
    }
    return 0;
}
bool    Autor::procurar(QString procura)
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

int     Musica::mudarNome(QString nome)
{
    /*
     *  Mudar o nome na BD
     * */

    QString final = _diretoria;
    final.chop(final.size() - final.lastIndexOf("/"));
    final = final + "/" + nome;

    QFile::copy(_diretoria,final);
    QFile::remove(_diretoria);
    _diretoria = final;
    _nome = nome;

    return 0;
}
int     Musica::mudarGenero(QString genero)
{
    return 0;
}
QDate   Musica::getDataAdicao()
{
    return _dataAdicao;
}
int     Musica::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}
int     Musica::getIdBD()
{
    return _idBD;
}
int     Musica::setIdBD(int id)
{
    _idBD = id;
    return 0;
}
QString Musica::getNome()
{
    return _nome;
}
int     Musica::setNome(QString nome)
{
    _nome = nome;

    return 0;
}
QString Musica::getDiretoria()
{
    return _diretoria;
}
int     Musica::setDiretoria(QString diretoria)
{
    _diretoria = diretoria;

    return 0;
}
int     Musica::getAutor(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = _autor->begin();
        current != _autor->end();
        ++current)
    {
        autores->append(*current);
    }

    return 0;
}
int     Musica::setAutor(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = autores->begin();
        current != autores->end();
        ++current)
    {
        _autor->append(*current);
    }

    return 0;
}
int     Musica::addAutor(Autor *autor)
{
    _autor->append(autor);
    return 0;
}
int     Musica::removeAutor(Autor *autor)
{
    if(this->hasAutor(autor))
    {
        _autor->removeOne(autor);
    }
    return 0;
}
bool    Musica::hasAutor(Autor *autor)
{
    return _autor->contains(autor);
}
QString Musica::getGenero()
{
    return _genero;
}
int     Musica::setGenero(QString genero)
{
    _genero = genero;

    return 0;
}
int     Musica::getFaixa()
{
    return _faixa;
}
int     Musica::setFaixa(int faixa)
{
    _faixa = faixa;
    return 0;
}
int     Musica::retrieveInfo(QString dir)
{
    /*
     * Método para leitura de tags id3
     * Ainda tenho de instalar a taglib ou lá o que é
     *
     * a biblioteca do Qt não funciona com mp3....
     * */
    QMediaPlayer musica;
    musica.setMedia(QUrl::fromLocalFile(dir));
    if(musica.isMetaDataAvailable())
    {
        _nome = musica.metaData("Title").toString();
        _genero = musica.metaData("Genre").toString();
        _faixa = musica.metaData("TrackNumber").toInt();
    }else{
        _nome = QUrl::fromLocalFile(dir).fileName();
        _genero = "Genero desconhecido";
        _faixa = 0;
    }

    return 0;
}
int     Musica::apagar()
{
    /*
     * Método para remover ficheiro
     * */
    return 0;
}
int     Musica::play()
{
    /*
     * Obsoleto? isto está a ser feito em mainwindow
     * */
    return 0;
}
int     Musica::criar(int albumID, QString diretoria)
{
    Database db;
    db.connOpen();
    QString file_name, new_dir; 
    file_name = _diretoria.right(_diretoria.size() - _diretoria.lastIndexOf("/"));
    new_dir   = diretoria + file_name;

    if(QFile::copy(_diretoria, new_dir))
    {
        _diretoria = new_dir;
        _dataAdicao = QDate::currentDate();
        db.addSong(this, albumID);
    }
    /* else if (remover e voltar a copiar)
*	 	_diretoria = new_dir;
        _dataAdicao = QDate::currentDate();
        db.addSong(this);
*/
    db.connClose();
    return 0;
}
bool    Musica::procurar(QString procura)
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

QDate   Album::getDataAdicao()
{
    return _dataAdicao;
}
int     Album::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}
int     Album::getIdBD()
{
    return _idBD;
}
int     Album::setIdBD(int id)
{
    _idBD = id;
    return 0;
}
QString Album::getNome()
{
    return _nome;
}
int     Album::setNome(QString nome)
{
    _nome = nome;

    return 0;
}
QString Album::getDescricao()
{
    return _descricao;
}
int     Album::setDescricao(QString descricao)
{
    _descricao = descricao;

    return 0;
}
QString Album::getDiretoria()
{
    return _diretoria;
}
int     Album::setDiretoria(QString diretoria)
{
    _diretoria = diretoria;

    return 0;
}
QString Album::getGenero()
{
    return _genero;
}
int     Album::setGenero(QString genero)
{
    _genero = genero;

    return 0;
}
QString Album::getImagem()
{
    return _imagem;
}
int     Album::setImagem(QString imagem)
{
    _imagem = imagem;

    return 0;
}
int     Album::getAno()
{
    return _ano;
}
int     Album::setAno(int ano)
{
    _ano = ano;
    return 0;
}
int     Album::getAutores(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = _autor->begin();
        current != _autor->end();
        ++current)
    {
        autores->append(*current);
    }

    return 0;
}
int     Album::setAutores(QList <Autor*> *autores)
{
    for(QList <Autor*>::iterator current = autores->begin();
        current != autores->end();
        ++current)
    {
        _autor->append(*current);
    }

    return 0;
}
bool    Album::hasAutor(Autor *autor)
{
    return _autor->contains(autor);
}
int     Album::getMusicas(QList<Musica *> *musicas)
{
    for(QList <Musica*>::iterator current = _musica->begin();
        current != _musica->end();
        ++current)
    {
        musicas->append(*current);
    }

    return 0;
}
int     Album::setMusicas(QList<Musica *> *musicas)
{
    for(QList <Musica*>::iterator current = musicas->begin();
        current != musicas->end();
        ++current)
    {
        _musica->append(*current);
    }

    return 0;
}
int     Album::apagar()
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
        if(removeDir(this->getDiretoria()))
        {
            qDebug() << "Album Apagado com Sucesso";
        }

        db.removeAlbum(this);
        db.connClose();
        return 0;
    }

    return -2;
}
int     Album::remover(Musica *musica)
{
    if(!_musica->contains(musica))
        return -1;

    _musica->removeOne(musica);

    return 0;
}
int     Album::remover(QList <Musica*> *musicas)
{
    for(int i = 0; i < musicas->size(); i++)
    {
        if(!_musica->contains(musicas->value(i)))
            return -1;

        _musica->removeOne(musicas->value(i));
    }
    return 0;
}
int     Album::play()
{
    /*
     * Obsoleto? isto está a ser feito em mainwindow
     * */
    return 0;
}
int     Album::adicionar(Musica *musica)
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

    /*
     * Adicionar ligação entre musica e album na BD
     * */

    return 0;
}
int     Album::criar()
{
    Database db;
    QString oldImgDir = this->getImagem();
    _dataAdicao = QDate::currentDate();

    if(db.addAlbum(this))
    {
        if(QDir(_diretoria).exists())
        {
            qDebug() << "Já existe: " << _diretoria;
        }
        else
        {
            qDebug() << "Criar " << _diretoria << "diretoria.";
            QDir().mkpath(_diretoria);
        }

        if(QDir(_diretoria).exists())
        {
            if(this->getImagem()!= NULL)
            {

                QImage img(oldImgDir);
                QPixmap pixmap;
                pixmap = pixmap.fromImage(img.scaled(250,250,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QFileInfo fi(this->getImagem());

                QFile file(this->getImagem());

                file.open(QIODevice::WriteOnly);
                if(fi.suffix()== "jpg")
                {
                    pixmap.save(&file, "jpg",100);
                }

                if(fi.suffix()== "png")
                {
                    pixmap.save(&file, "png",100);
                }

                file.close();
            }
        }

    }
    else
    {
        return -1;
    }

    return 0;
}
bool    Album::procurar(QString procura)
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

QDate   Playlist::getDataAdicao()
{
    return _dataAdicao;
}
int     Playlist::setDataAdicao(QDate data)
{
    _dataAdicao = data;
    return 0;
}
int     Playlist::getIdBD()
{
    return _idBD;
}
int     Playlist::setIdBD(int id)
{
    _idBD = id;
    return 0;
}
QString Playlist::getNome()
{
    return _nome;
}
int     Playlist::setNome(QString nome)
{
    _nome = nome;
    return 0;
}
QString Playlist::getDescricao()
{
    return _descricao;
}
int     Playlist::setDescricao(QString descricao)
{
    _descricao = descricao;
    return 0;
}
int     Playlist::getSize()
{
    return _musica->size();
}
int     Playlist::getMusicas(QList<Musica *> *musicas)
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
int     Playlist::getIndiceDe(Musica *musica)
{
    return _musica->indexOf(musica);
}
int     Playlist::setMusicas(QList<Musica *> *musicas)
{
    for(QList <Musica*>::iterator current = musicas->begin();
        current != musicas->end();
        ++current)
    {
        _musica->append(*current);
    }

    return 0;
}
int     Playlist::apagar()
{
    /*
     * Apagar informação na BD
     * */
    Database db;
    db.connOpen();
    db.removePlaylist(this);
    db.connClose();
    return 0;
}
int     Playlist::play()
{
    /*
     * Obsoleto? isto está a ser feito em mainwindow
     * */
    return 0;
}
int     Playlist::adicionar(Musica *musica)
{
    if(_musica->contains(musica) || musica == NULL)
        return -1;

    _musica->append(musica);

    return 0;
}
int     Playlist::adicionar(QList<Musica*> *musica)
{
    for(QList <Musica*>::iterator current = musica->begin();
        current != musica->end();
        ++current)
    {
        if(!_musica->contains(*current))
        {
            _musica->append(*current);
        }
    }



    return 0;
}
int     Playlist::remover(Musica *musica)
{
    if(!_musica->contains(musica))
    {
        return -1;
    }

    _musica->removeOne(musica);

    return 0;
}
int     Playlist::criar()
{
    /*
     * Introduzir informação na BD
    */
    Database db;
    db.connOpen();
    db.addPlaylist(this);
    db.connClose();
    return 0;
}
bool    Playlist::procurar(QString procura)
{
    if(_nome.contains(procura,Qt::CaseInsensitive))
    {
        return true;
    }
    return false;
}

// ================================================
// Metodos classe Player

Player::Player()
{
    _lista = new Playlist;
    _lista->setNome("Lista de Reprodução");
    _lista->setDescricao("Lista de reprodução de player");
    _aleatorio = false;
    _repeat = false;
    _aTocar = false;
    _mediaPlayer = new QMediaPlayer;
    _reproducao = new QMediaPlaylist;
    _mediaPlayer->setPlaylist(_reproducao);
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
    _mediaPlayer->play();
    _aTocar = true;

    return 0;
}

int Player::pausa()
{
    _mediaPlayer->pause();
    _aTocar = false;

    return 0;
}

int Player::parar()
{
    _mediaPlayer->stop();
    _aTocar = false;

    return 0;
}

int Player::seguinte()
{
    _reproducao->next();

    return 0;
}

int Player::anterior()
{
    _reproducao->previous();

    return 0;
}

int Player::aleatorio(bool aleatorio)
{
    if(_aleatorio != aleatorio)
    {
        _aleatorio = aleatorio;
        if(_aleatorio)
        {
            _repeat = false;
            _reproducao->setPlaybackMode(QMediaPlaylist::Random);
        }else{
            _reproducao->setPlaybackMode(QMediaPlaylist::Sequential);
        }
    }
    return 0;
}

int Player::repetir(bool repetir)
{
    if(_repeat != repetir)
    {
        _repeat = repetir;
        if(_repeat)
        {
            _aleatorio = false;
            _reproducao->setPlaybackMode(QMediaPlaylist::Loop);
        }else{
            _reproducao->setPlaybackMode(QMediaPlaylist::Sequential);
        }
    }
    return 0;
}

int Player::silencio(bool silencio)
{
    if(_silencio != silencio)
    {
        _silencio = silencio;
        _mediaPlayer->setMuted(true);
    }
    return 0;
}

int Player::adicionar(QList <Musica*> *musica)
{
    Musica *song;
    for(QList <Musica*>::iterator current = musica->begin();
        current != musica->end();
        ++current)
    {
        song = *current;
        _lista->adicionar(song);
        _reproducao->addMedia(QUrl::fromLocalFile(song->getDiretoria()));
    }
    return 0;
}

int Player::adicionar(Musica *musicas)
{
    _lista->adicionar(musicas);
    _reproducao->addMedia(QUrl::fromLocalFile(musicas->getDiretoria()));
    return 0;
}

int Player::remover(QList<Musica *> *musica)
{
    if(musica->isEmpty())
        return -1;

    int index;
    for(QList <Musica*>::iterator current = musica->begin();
        current != musica->end();
        ++current)
    {
        index = _lista->getIndiceDe(*current);
        _lista->remover(*current);
        _reproducao->removeMedia(index);
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
    _reproducao->clear();

    return 0;
}

bool Player::isEmpty()
{
    return _lista->getSize() == 0;
}

bool Player::isAleatorio()
{
    return _aleatorio;
}


bool removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
bool Player::isRepeat()
{
    return _repeat;
}

bool Player::isSilencio()
{
    return _silencio;
}

bool Player::isATocar()
{
    return _aTocar;
}

qint64 Player::getTempo()
{
    return _mediaPlayer->duration();
}

qint64 Player::getPosicao()
{
    return _mediaPlayer->position();
}

int Player::setPosicao(qint64 posicao)
{
    _mediaPlayer->setPosition(posicao);
    return 0;
}
