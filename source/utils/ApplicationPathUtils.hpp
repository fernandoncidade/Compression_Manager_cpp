#pragma once

#include <QString>

QString get_app_base_path();
QString getAppBasePath();

QString get_text_file_path(const QString& filename, const QString& folder = QString());
QString getTextFilePath(const QString& filename, const QString& folder = QString());

QString load_text_file(const QString& filename, const QString& folder = QString(), const QString& encoding = "utf-8");
QString loadTextFile(const QString& filename, const QString& folder = QString(), const QString& encoding = "utf-8");
