// Dupla:
// Matheus Fernandes Bigolin (22200371)
// Pedro Henrique De Sena Trombini Taglialenha (22203674)

#include "dicio.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

std::ifstream abrir_arquivo(std::string nome_arq) {
  std::ifstream arq(nome_arq, std::ios_base::binary | std::ios_base::in);

  if (!arq.is_open()) {
    throw std::runtime_error("Não foi possível abrir " + nome_arq);
  }

  return arq;
}

int main(void) {
  std::string nome_arq;
  std::cin >> nome_arq;

  auto arq_dicio{ abrir_arquivo(nome_arq) };

  Dicionario dicio{};

  int tamanho_total{ 0 };
  std::string verbete;
  while (std::getline(arq_dicio, verbete)) {
    // Encontra o início e o fim da palavra sendo definida delimitada por
    // colchetes.
    const std::size_t inicio{ verbete.find('[') };
    const std::size_t fim{ verbete.find(']', inicio + 2) };

    if (inicio != std::string::npos && fim != std::string::npos) {
      // Extração da palavra que está sendo definida.
      std::string palavra{ verbete.substr(inicio + 1, fim - inicio - 1) };

      dicio.inserir(palavra, tamanho_total + inicio + 1, verbete.size());
    } else {
      throw std::runtime_error("Verbete mal-formado.");
    }

    tamanho_total += verbete.size() + 1;
  }

  while (true) {
    std::string palavra{};

    std::cin >> palavra;

    if (palavra == "0") {
      break;
    }

    if (dicio.existe_prefixo(palavra)) {
      std::size_t contagem{ dicio.contar_palavras_prefixadas(palavra) };

      std::cout << palavra << " is prefix of " << contagem << " words"
                << std::endl;

      Dicionario::Indice indice{ dicio.obter_indexacao(palavra) };

      // Verifica se o índice é valido antes de imprimi-lo.
      if (indice.posicao != 0 && indice.tamanho != 0) {
        std::cout << palavra << " is at (" << indice.posicao - 1 << ","
                  << indice.tamanho << ")" << std::endl;
      }
    } else {
      std::cout << palavra << " is not prefix." << std::endl;
    }
  }

  arq_dicio.close();
  return EXIT_SUCCESS;
}
