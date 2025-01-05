// Dupla:
// Matheus Fernandes Bigolin (22200371)
// Pedro Henrique De Sena Trombini Taglialenha (22203674)

#include <cstddef>
#include <string>

// Número de letras no alfabeto empregado.
constexpr auto TAMANHO_ALFABETO{ 'z' - 'a' + 1 };

class Dicionario {
private:
  // Um nó representa uma letra em uma posição específica.
  struct No {
    // Nós que sucedem o nó atual.
    No *filhos[TAMANHO_ALFABETO] = {};

    // Indica se o nó representa a última letra de uma palavra do dicionário.
    bool esta_terminal = false;

    // Posição e tamanho de um verbete no dicionário. Somente válidos se
    // esta_terminal é verdadeiro.
    std::size_t posicao = 0;
    std::size_t tamanho = 0;

    ~No() {
      for (auto filho : filhos) {
        delete filho;
      }
    }
  };

  // Início do dicionário; não representa nenhuma letra.
  No *raiz{ nullptr };

public:
  Dicionario() : raiz(new No()) {
  }

  ~Dicionario() {
    delete raiz;
  }

  void inserir(const std::string &palavra, std::size_t posicao,
               std::size_t tamanho) {
    No *atual{ raiz };

    for (auto letra : palavra) {
      if (atual->filhos[letra - 'a'] == nullptr) {
        atual->filhos[letra - 'a'] = new No();
      }

      atual = atual->filhos[letra - 'a'];
    }

    atual->esta_terminal = true;
    atual->posicao = posicao;
    atual->tamanho = tamanho;
  }

  bool existe_prefixo(const std::string &prefixo) const {
    return encontrar_prefixo(prefixo) != nullptr;
  }

  std::size_t contar_palavras_prefixadas(const std::string &prefixo) const {
    return contar_palavras(encontrar_prefixo(prefixo));
  }

  // Índice formado pela posição e tamanho de um verbete no dicionário.
  struct Indice {
    std::size_t posicao;
    std::size_t tamanho;
  };

  // Obtém a indexação de uma determinada palavra no dicionário.
  Indice obter_indexacao(const std::string &palavra) const {
    No *no{ encontrar_prefixo(palavra) };

    // Prefixo não existe ou não forma uma palavra.
    if (no == nullptr || !no->esta_terminal) {
      return Indice{ 0, 0 };
    }

    return Indice{ no->posicao, no->tamanho };
  }

private:
  // Encontra a subárvore de um determinado prefixo. Caso esse prefixo não
  // exista, retorna o ponteiro nulo.
  No *encontrar_prefixo(const std::string &prefixo) const {
    No *atual{ raiz };

    for (auto letra : prefixo) {
      if (atual->filhos[letra - 'a'] == nullptr) {
        return nullptr;
      }

      atual = atual->filhos[letra - 'a'];
    }

    return atual;
  }

  // Conta o número de todas as palavras na subárvore.
  std::size_t contar_palavras(No *no) const {
    if (no == nullptr) {
      return 0;
    }

    std::size_t contagem{ 0 };

    if (no->esta_terminal) {
      contagem++;
    }

    for (No *filho : no->filhos) {
      contagem += contar_palavras(filho);
    }

    return contagem;
  }
};
