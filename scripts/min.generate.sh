#!/bin/bash

# Directorio de origen que contiene los archivos de código fuente (.cpp) y cabeceras (.hpp)
directorio_origen="src"

# Directorio de destino para el archivo combinado y la carpeta min
directorio_destino="min"

# Crear directorio de destino si no existe
mkdir -p "$directorio_destino"

# Crear el archivo combinado veridic.min.hpp
archivo_combinado="$directorio_destino/veridic.min.hpp"
touch "$archivo_combinado"

# Iterar sobre todos los archivos en el directorio de origen
for archivo in "$directorio_origen"/{headers/http/*.hpp,headers/utils/*.hpp,sources/**/*.cpp}; do
    # Verificar si el archivo es .cpp o .hpp
    if [[ "$archivo" == *.cpp ]]; then
        tipo_archivo="cpp"
    elif [[ "$archivo" == *.hpp ]]; then
        tipo_archivo="hpp"
    else
        continue  # Saltar archivos que no sean .cpp o .hpp
    fi

    # Obtener la ruta del directorio del archivo actual
    directorio_archivo=$(dirname "$archivo")

    # Excluir los archivos dentro del directorio "main"
    if [[ "$directorio_archivo" == *"main"* ]]; then
        continue
    fi

    # Obtener nombre del archivo sin la ruta ni extensión
    nombre_archivo=$(basename -- "$archivo")
    nombre_archivo_sin_extension="${nombre_archivo%.*}"
    # Imprimir separador en el archivo combinado
    echo -e "\n// Archivo: $nombre_archivo\n" >> "$archivo_combinado"
    # Imprimir contenido del archivo en el archivo combinado
    cat "$archivo" >> "$archivo_combinado"
done

# Eliminar includes que utilizan comillas dobles
sed -i '/#include *"[^"]*"/d' "$archivo_combinado"