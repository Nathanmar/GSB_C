#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEDICAMENTS 100

typedef struct {
    char marque[50];
    char nom[50];
} Medicament;

void afficherMedicaments(Medicament* medicaments, int nombreMedicaments) {
    for (int i = 0; i < nombreMedicaments; i++) {
        printf("Marque: %s, Nom: %s\n", medicaments[i].marque, medicaments[i].nom);
    }
}

void filtrerParMarque(Medicament* medicaments, int nombreMedicaments, const char* marque) {
    printf("Médicaments de la marque %s :\n", marque);
    for (int i = 0; i < nombreMedicaments; i++) {
        if (strcmp(medicaments[i].marque, marque) == 0) {
            printf("Nom: %s\n", medicaments[i].nom);
        }
    }
}

int main() {
    FILE* fichier = fopen("medicaments.json", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return ;
    }

    char ligne[256];
    char contenu[1024];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        strcat(contenu, ligne);
    }

    fclose(fichier);

    // Analyser le contenu JSON et remplir le tableau de médicaments
    const char* marque_key = "\"marque\":";
    const char* nom_key = "\"nom\":";

    char* marque_start = strstr(contenu, marque_key);
    char* nom_start = strstr(contenu, nom_key);

    Medicament medicaments[MAX_MEDICAMENTS];
    int nombreMedicaments = 0;

    while (marque_start != NULL && nom_start != NULL && nombreMedicaments < MAX_MEDICAMENTS) {
        marque_start += strlen(marque_key);
        nom_start += strlen(nom_key);

        sscanf(marque_start, "\"%49[^\"]", medicaments[nombreMedicaments].marque);
        sscanf(nom_start, "\"%49[^\"]", medicaments[nombreMedicaments].nom);

        nombreMedicaments++;

        marque_start = strstr(marque_start, marque_key);
        nom_start = strstr(nom_start, nom_key);
    }

    // Afficher tous les médicaments
    afficherMedicaments(medicaments, nombreMedicaments);

    // Filtrer par marque
    const char* marqueRecherchee = "Paracétamol";
    filtrerParMarque(medicaments, nombreMedicaments, marqueRecherchee);

    return 0;
}
