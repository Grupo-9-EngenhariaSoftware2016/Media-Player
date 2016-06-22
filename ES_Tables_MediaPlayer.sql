# Albuns
CREATE TABLE [Album](
    [ID_Album] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [Nome] TEXT NOT NULL, 
    [Descricao] TEXT, 
    [Diretoria] TEXT, 
    [Imagem] TEXT, 
    [Ano] INTEGER, 
    [Genero] TEXT DEFAULT Desconhecido, 
    [DataAdicao] DATE);
#Autor
CREATE TABLE [Autor](
    [ID_Autor] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [Nome] TEXT NOT NULL, 
    [Nacionalidade] TEXT, 
    [DataNascimento] DATE, 
    [Imagem] TEXT, 
    [DataAdicao] DATE);
#Musica
CREATE TABLE [Musica](
    [ID_Musica] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [Nome] TEXT NOT NULL, 
    [Diretoria] TEXT NOT NULL, 
    [Faixa] INTEGER, 
    [ID_Album] INTEGER NOT NULL, 
    FOREIGN KEY([ID_Album]) REFERENCES Album([ID_Album]) ON DELETE CASCADE ON UPDATE CASCADE);
#Playlist
CREATE TABLE [Playlist](
    [ID_Playlist] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [Nome] TEXT NOT NULL, 
    [Descricao] TEXT, 
    [DataAdicao] DATE);
#Pertence (Musica Pertencente a uma Playlist)
CREATE TABLE [Pertence](
    [ID_Musica] INTEGER NOT NULL, 
    [ID_Playlist] INTEGER NOT NULL, 
    FOREIGN KEY([ID_Playlist]) REFERENCES Playlist([ID_Playlist]), 
    FOREIGN KEY([ID_Musica]) REFERENCES Musica([ID_Musica]));
#Tem (Associar Musica aos Autores)
CREATE TABLE [Tem](
    [ID_Musica] INTEGER PRIMARY KEY NOT NULL, 
    [ID_Autor] INTEGER NOT NULL, 
    FOREIGN KEY([ID_Autor]) REFERENCES Autor([ID_Autor]), 
    FOREIGN KEY([ID_Musica]) REFERENCES Musica([ID_Musica]));
