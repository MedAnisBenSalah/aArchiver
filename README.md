# aArchiver
aArchiver is a basic file archiver and encoder with a fully functioning GUI.

<p align="center">
  <a href="#introduction">Introduction</a> •
  <a href="#components">Components</a> •
  <a href="#aaa-file-extension">AAA File extension</a> •
  <a href="#media">Media</a>
</p>

# Introduction

This archiver was originally developed as an extension for a C++ game engine, but as the game engine's developement finally came to an end, 
the archiver component was re-written and released for anyone to benefit from.

# Components

<p align="center">
  <a href="#generator">Archiver</a> •
  <a href="#encoder">Encoder</a> •
  <a href="#gui">GUI</a>
</p>

## Archiver

Responsable for writing and reading into and from the archive file, can archive multiple files at once or one at a time, can also read and
extract files or file information (creation date, icon, size ...) from any given AAA archive.

Please refer to the <a href="https://github.com/MedAnisBenSalah/aArchiver/blob/master/Compressor/Archiver.cpp">source file</a> for more insights.

## Encoder

Based on <a href="https://github.com/ReneNyffenegger">Rene Nyffenegger</a>'s <a href="https://github.com/ReneNyffenegger/cpp-base64">cpp-base64</a> project, this will encode every single file archived for a better
protection.

Please refer to the <a href="https://github.com/MedAnisBenSalah/aArchiver/blob/master/Shared/Encoder.cpp">source file</a> for more insights.

## GUI

Contains a full Win32 based GUI, will use the archive's files information (icon, size and creation date) and display them into a listview, please
refer to the <a href="#media">Media section</a> for a better view.

# AAA File Extension

## Why

A custom file extension storing file's data with a header.

## Header

Every archive file is composed of a header at the top of the file containing:

* version (int32): A 32-bit integer version number, commonly referring to the archiver's version.
* files count (int64): A 64-bit long integer files count number, commonly referring to the number of files within the archive.
* is encoded (boolean): A 4-bit boolean flag, indicating if the archive is encoded.

## File structure

After the header, every single file is written with the following structure:

* name length (int32): A 32-bit unsigned integer, indicating the file name's length.
* file name (string): A string of a determined length, representing the file's name.
* icon length (int32): A 32-bit unsigned integer, indicating the icon's data length.
* icon bitmap data (array): A bytes-array of a determined length, representing if the icon's bitmap data.
* type length (int32): A 32-bit unsigned integer, indicating the file type's length.
* file type (string): A string of a determined length, representing the file's type (extension).
* creation date (<a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms724950(v=vs.85).aspx">SYSTEMTIME</a>): A structure of a determined size, containing information about the file's creation date.
* last modified date (<a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms724950(v=vs.85).aspx">SYSTEMTIME</a>): A structure of a determined size, containing information about the file's last modified date.
* data length (int32): A 32-bit unsigned integer, indicating the file's content date length.
* file data (array): A bytes-array of a determined length, representing the file's contents (data).

# Media

<p align=center><b>Interface view</b></p>

![General interface](https://github.com/MedAnisBenSalah/aArchiver/blob/master/Media/screen1.png)
![Extraction interface](https://github.com/MedAnisBenSalah/aArchiver/blob/master/Media/screen2.png)
![Context menu](https://github.com/MedAnisBenSalah/aArchiver/blob/master/Media/screen3.png)


