// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "glass/networktables/NTStringChooser.h"

#include <fmt/format.h>

using namespace glass;

NTStringChooserModel::NTStringChooserModel(std::string_view path)
    : NTStringChooserModel{nt::NetworkTableInstance::GetDefault(), path} {}

NTStringChooserModel::NTStringChooserModel(nt::NetworkTableInstance inst,
                                           std::string_view path)
    : m_inst{inst},
      m_default{
          m_inst.GetStringTopic(fmt::format("{}/default", path)).Subscribe("")},
      m_selected{
          m_inst.GetStringTopic(fmt::format("{}/selected", path)).GetEntry("")},
      m_active{
          m_inst.GetStringTopic(fmt::format("{}/active", path)).Subscribe("")},
      m_options{m_inst.GetStringArrayTopic(fmt::format("{}/options", path))
                    .Subscribe({})} {
  m_selected.GetTopic().SetRetained(true);
}

void NTStringChooserModel::SetSelected(std::string_view val) {
  m_selected.Set(val);
}

void NTStringChooserModel::Update() {
  if (!m_default.Exists()) {
    m_defaultValue.clear();
  }
  for (auto&& v : m_default.ReadQueue()) {
    m_defaultValue = std::move(v.value);
  }

  if (!m_selected.Exists()) {
    m_selectedValue.clear();
  }
  for (auto&& v : m_selected.ReadQueue()) {
    m_selectedValue = std::move(v.value);
  }

  if (!m_active.Exists()) {
    m_activeValue.clear();
  }
  for (auto&& v : m_active.ReadQueue()) {
    m_activeValue = std::move(v.value);
  }

  if (!m_options.Exists()) {
    m_optionsValue.clear();
  }
  for (auto&& v : m_options.ReadQueue()) {
    m_optionsValue = std::move(v.value);
  }
}

bool NTStringChooserModel::Exists() {
  return m_inst.IsConnected() && m_options.Exists();
}
