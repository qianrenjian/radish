/*
 * File: transformer_encoder.h
 * Project: transformer
 * Author: koth (Koth Chen)
 * -----
 * Last Modified: 2019-09-17 9:55:29
 * Modified By: koth (nobody@verycool.com)
 * -----
 * Copyright 2020 - 2019
 */
#pragma once

#include <cstddef>
#include <vector>

#include "torch/nn/cloneable.h"
#include "torch/nn/module.h"
#include "torch/nn/modules/container/any.h"
#include "torch/nn/modules/container/modulelist.h"
#include "torch/nn/modules/embedding.h"
#include "torch/nn/pimpl.h"
#include "torch/types.h"

#include "radish/transformer/decoder_layer.h"

namespace radish {
using Tensor = torch::Tensor;
/// Options for the `TransformerDecoder` module.
struct TORCH_API TransformerDecoderOptions {
  TransformerDecoderOptions(int64_t n_tgt_vocab, int64_t len_max_seq,
                            int64_t d_word_vec, int64_t n_layers,
                            int64_t n_head, int64_t d_k, int64_t d_v,
                            int64_t d_model, int64_t d_inner,
                            double dropout = 0.1);
  TORCH_ARG(int64_t, n_tgt_vocab);
  TORCH_ARG(int64_t, len_max_seq);
  TORCH_ARG(int64_t, d_word_vec);
  TORCH_ARG(int64_t, n_layers);
  TORCH_ARG(int64_t, n_head);
  TORCH_ARG(int64_t, d_k);
  TORCH_ARG(int64_t, d_v);
  TORCH_ARG(int64_t, d_model);
  TORCH_ARG(int64_t, d_inner);
  TORCH_ARG(double, dropout) = 0.1;
};

class TORCH_API TransformerDecoderImpl
    : public ::torch::nn::Cloneable<TransformerDecoderImpl> {
 public:
  TransformerDecoderImpl(int64_t n_tgt_vocab, int64_t len_max_seq,
                         int64_t d_word_vec, int64_t n_layers, int64_t n_head,
                         int64_t d_k, int64_t d_v, int64_t d_model,
                         int64_t d_inner, double dropout = 0.1)
      : TransformerDecoderImpl(TransformerDecoderOptions(
            n_tgt_vocab, len_max_seq, d_word_vec, n_layers, n_head, d_k, d_v,
            d_model, d_inner, dropout)) {}
  explicit TransformerDecoderImpl(TransformerDecoderOptions options);

  void reset() override;

  void pretty_print(std::ostream& stream) const override;

  std::vector<Tensor> forward(const Tensor& tgt_seq, const Tensor& tgt_pos,
                              const Tensor& src_seq, const Tensor& enc_output,
                              bool return_attns = false);

  /// The options used to configure this module.
  TransformerDecoderOptions options;
  torch::nn::Embedding tgt_word_emb = nullptr;
  torch::nn::Embedding pos_emb = nullptr;
  torch::nn::ModuleList decoder_stack = {};
};

/// A `ModuleHolder` subclass for `TransformerDecoderImpl`.
/// See the documentation for `TransformerDecoderImpl` class to learn what
/// methods it provides, or the documentation for `ModuleHolder` to learn about
/// PyTorch's module storage semantics.
TORCH_MODULE(TransformerDecoder);

}  // namespace radish
