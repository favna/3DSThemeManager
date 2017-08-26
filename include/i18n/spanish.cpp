// Copyright (C) 2017 ErmanSayin
// 
// This file is part of Themely.
// 
// Themely is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Themely is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Themely.  If not, see <http://www.gnu.org/licenses/>.
// 

#include <string>
#include <map>

// Translated by umbjolt

std::map<std::string, std::wstring> SPANISH = {
	// UI.CPP
	{"scanning",            L"Escaneando temas..."},
	{"error_message",       L"Se ha producido un error. Tu versión de 3DSThemeManager es la %s."},
	{"press_start",         L"Pulsa START para salir."},
	{"themes",              L"%i tema%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"Escaner QR"},
	{"by",                  L"por %ls"},
	{"add_to_shuffle",      L"Añadir a aleatorios"},
	{"remove_from_shuffle", L"Eliminar de aleatorios"},
	{"install",             L"Instalar"},
	{"w_o_bgm",             L"sin BGM"},
	{"fullscreen_prev",     L"Pantalla completa"},
	{"hold_delete",         L"[mantener] eliminar"},
	{"bgm_prev",            L"Previsualizar BGM"},
	{"installing",          L"Instalando..."},
	{"downloading",         L"Descargando..."},
	{"new_update",          L"¡Hay una nueva actualización!"},
	{"delete_prompt_1",     L"¿Estás seguro de que quieres"},
	{"delete_prompt_2",     L"eliminar este tema?"},
	{"dump_prompt",         L"¿Volcar el tema actualmente instalado?"},
	{"dump_prompt_warn",    L"¡Por favor, no subáis temas oficiales a ThemePlaza!"},
	{"playing",             L"Reproduciendo..."},
	{"no_themes_1",         L"¡No se han encontrado temas!"},
	{"no_themes_2",         L"Ve a ThemePlaza en tu ordenador, descarga algunos temas,"},
	{"no_themes_3",         L"y ponlos en la carpeta /Themes de tu tarjeta SD."},
	{"shuffle_count",       L"%i/10 tema%ls seleccionados para el modo aleatorio"},
	{"qr_explain_1",        L"Escanea un código QR del enlace directo"},
	{"qr_explain_2",        L"de un archivo ZIP de un tema para descargarlo."},
	{"qr_explain_3",        L"Ve a ThemePlaza, elije un tema, luego pulsa en el"},
	{"qr_explain_4",        L"botón del código QR para ver el QR de ese tema."},
	{"update_prompt",       L"¿Instalar actualización?"},
	{"yes",                 L"Sí"},
	{"no",                  L"No"},
	{"stop_bgm",            L"Pulsa (Y) para parar."},
	// THEME.CPP
	{"no_desc",             L"[descripción no disponible]"},
	{"unknown",             L"Desconocido"},
	{"install_reading",     L"Leyendo %s..."},
	{"install_writing",     L"Escribiendo en %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Fallo al reproducir el BGM"},
	// UI.CPP
	{"err_texture",        L"Fallo al cargar texturas."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Fallo al descargar actualización."},
	{"err_update_manual",  L"Si aún estás teniendo problemas, actualiza manualmente."},
	{"err_update_titledb", L"Si aún estás teniendo problemas, actualiza desde FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Fallo al descargar el archivo ZIP"},
	{"err_zip_invalid",    L"El archivo ZIP es inválido"},
	{"err_zip_no_body",    L"El archivo ZIP no contiene un body_LZ.bin en la raíz"},
	// THEME.CPP
	{"err_fail_open",      L"Fallo al abrir %s."},
	{"err_fail_read",      L"Fallo al leer %s."},
	{"err_fail_write",     L"Fallo al escribir en %s."},
	{"err_try_default",    L"Intenta selecionar uno de los temas por defecto en los ajustes del menú home antes de intentarlo de nuevo"},
	// err_bgm_too_big
	{"err_no_dsp",         L"¡DSP no encontrado! Usa DSP1 para hacer un volcado del firmware DSP requerido para la reprodución de audio."}
};
