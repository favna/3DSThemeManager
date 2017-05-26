// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

// Translated by Erman SAYIN
// Traduit par Erman SAYIN

#include <string>
#include <map>

std::map<std::string, std::wstring> FRENCH = {
	// UI.CPP
	{"scanning",            L"Indexage en cours..."},
	{"error_message",       L"Une erreur est survenue. Vous avez Themely version %s."},
	{"press_start",         L"Appuyez sur START pour quitter."},
	{"themes",              L"%i thème%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"Scanner de code QR"},
	{"by",                  L"par %s"},
	{"add_to_shuffle",      L"Ajoute au aléatoire"},
	{"remove_from_shuffle", L"Retire de aléatoire"},
	{"install",             L"Installer"},
	{"w_o_bgm",             L"sans BGM"},
	{"fullscreen_prev",     L"Aperçu en plain écran"},
	{"hold_delete",         L"[tenir] Effacer"},
	{"bgm_prev",            L"Aperçu BGM"},
	{"installing",          L"Installation..."},
	{"downloading",         L"Téléchargement..."},
	{"new_update",          L"Nouvelle mise à jour disponible!"},
	{"delete_prompt_1",     L"Êtes-vous certain(e) de vouloir"},
	{"delete_prompt_2",     L"effacer ce thème?"},
	{"dump_prompt",         L"Extraire le thème installé?"},
	{"dump_prompt_warn",    L"Ne soumettez pas des thèmes officiels à 3DSThem.es SVP!"},
	{"playing",             L"Lecture..."},
	{"no_themes_1",         L"Aucun thème trouvé!"},
	{"no_themes_2",         L"Allez sur 3DSThem.es, téléchargez des thèmes,"},
	{"no_themes_3",         L"et les transférez sur le fichier /Themes dans votre carte SD."},
	{"shuffle_count",       L"%i/10 thème%ls sélectionnés pour aléatoire"},
	{"qr_explain_1",        L"Scanner un code QR d'un lien directe"},
	{"qr_explain_2",        L"à un fichier ZIP d'un thème pour la télécharger."},
	{"qr_explain_3",        L"Allez sur 3DSThem.es, sélectionez un thème, et cliquez sur"},
	{"qr_explain_4",        L"le bouton code QR pour voir le code QR pour ce thème."},
	{"update_prompt",       L"Installer la mise à jour?"},
	{"yes",                 L"Oui"},
	{"no",                  L"Non"},
	{"stop_bgm",            L"Appuiez sur (Y) pour arrêter la musique."},
	// THEME.CPP
	{"no_desc",             L"[description non disponible]"},
	{"unknown",             L"Inconnu"},
	{"install_reading",     L"Lecture de %s..."},
	{"install_writing",     L"Écriture sur %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Échec de la lecture de la musique."},
	// UI.CPP
	{"err_texture",        L"Échec de lecture de textures."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Le téléchargement de la mise à jour a échoué."},
	{"err_update_manual",  L"Si la problème persiste, effectuez la mise à jour manuellement."},
	{"err_update_titledb", L"Si la problème persiste, mise à jour sur FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Le téléchargement de l'archive ZIP a échoué."},
	{"err_zip_invalid",    L"L'archive ZIP est invalide"},
	{"err_zip_no_body",    L"L'archive ZIP ne contient pas un fichier body_LZ.bin"},
	// THEME.CPP
	{"err_fail_open",      L"Échec de l'ouverture de %s."},
	{"err_fail_read",      L"Échec de lecture de %s."},
	{"err_fail_write",     L"Échec de écriture à %s."},
	{"err_try_default",    L"Essaye de sélectionner l'un des thèmes défauts sur les paramètres du menu HOME avant de essaier à nouveau"},
	{"err_no_dsp",         L"Le dump DSP n'a pas été trouvé! Utilise DspDump pour extraire le firmware DSP requise pour la lecture de l'audio."}
};
