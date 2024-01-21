class Regle():
    def __init__(self):
        self.liste_premisses = []
        self.liste_conclusions = []

    def ajouter_premisse(self, regle):
        self.liste_premisses.append(regle)

    def ajouter_conclusion(self, regle):
        self.liste_conclusions.append(regle)

    def __str__(self):
        return str(self.liste)

    def nbr_premisse(self):
        return len(self.liste_premisses)
    
    def afficher(self):
        return str(self.liste_premisses) + " => " + str(self.liste_conclusions)

    def supprimer_premisse(self, regle):
        self.liste_premisses.remove(regle)


class BaseDeRegles():
    def __init__(self):
        self.liste = []

    def ajouter_regle(self, regle):
        self.liste.append(regle)

    def nbr_regle(self):
        return len(self.liste)
    
    def afficher(self):
        for i, regle in enumerate(self.liste):
            print(f"Règle {i+1}: {regle.afficher()}")


def moteur(base_fait, base_connaissances):
    for fait in base_fait:
        for regle in base_connaissances.liste:
            if fait in regle.liste_premisses:
                regle.supprimer_premisse(fait)
                if regle.nbr_premisse() == 0:
                    base_fait.append(regle.liste_conclusions[0])
                    print("Ajout de " + regle.liste_conclusions[0] + " dans la base de fait")
                    base_connaissances.liste.remove(regle)


def lire_fichier_regles(chemin_fichier):
    base_connaissances = BaseDeRegles()
    base_fait = []
    with open(chemin_fichier, 'r') as fichier:
        for ligne in fichier:
            ligne = ligne.strip()

            # Ignorer les lignes vides ou celles commençant par '#'
            if not ligne or ligne.startswith('#'):
                continue

            # Lire une règle
            if ligne.startswith(('r', 'R')):
                # Séparer le nom de la règle (s'il y a) et le reste de la ligne
                if ':' in ligne:
                    nom_regle, reste = ligne[1:].split(':', 1)
                    nom_regle = nom_regle.strip()
                else:
                    nom_regle, reste = '', ligne[1:].strip()

                # Séparer les prémisses et les conclusions
                prem_concl = [part.strip() for part in reste.split(';')]

                # Créer une instance de la classe Regle et l'ajouter à la base de règles
                nouvelle_regle = Regle()
                if nom_regle:
                    nouvelle_regle.nom = nom_regle
                nouvelle_regle.liste_premisses = [premisse.strip() for premisse in prem_concl[0].split(',')]
                nouvelle_regle.liste_conclusions = [conclusion.strip() for conclusion in prem_concl[1].split(',')]
                base_connaissances.ajouter_regle(nouvelle_regle)

            # Lire les faits
            elif ligne.startswith(('f', 'F')):
                faits = [fait.strip() for fait in ligne[1:].split(':')[1].split(',')]

    return base_connaissances, faits


base_connaissances, base_faits = lire_fichier_regles("save.txt")
print(base_connaissances.nbr_regle())
base_connaissances.afficher()


moteur(base_faits, base_connaissances)

base_connaissances.afficher()
print(f"Base de faits : {base_faits}")