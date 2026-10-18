#pragma once

// system includes
#include <format>

// 3rdparty lib includes
#include <espchrono.h>
#include <espstrutils.h>
#include <espwifiutils.h>
#include <strutils.h>

// local includes
#include "richtexthelper.h"
#include "textinterface.h"

namespace espgui {

template<const char *Tprefix, typename Taccessor>
class TextWithValueHelper : public Taccessor, public virtual TextInterface
{
    using Taccessor::Taccessor;

public:
    [[nodiscard]] std::string text() const override
    {
        using cpputils::toString;
        using espchrono::toString;
        using espcpputils::toString;
        using wifi_stack::toString;

        return std::format("{} {}", Tprefix, richTextEscape(toString(Taccessor::getValue())));
    }
};

template<typename Taccessor>
class ChangeableTextWithValueHelper : public Taccessor, public virtual TextInterface
{
    using Taccessor::Taccessor;

public:
    [[nodiscard]] std::string text() const override
    {
        using cpputils::toString;
        using espchrono::toString;
        using espcpputils::toString;
        using wifi_stack::toString;

        return std::format("{} {}", m_prefix, richTextEscape(toString(Taccessor::getValue())));
    }

    [[nodiscard]] const std::string &prefix() const { return m_prefix; }
    void setPrefix(const std::string_view prefix) { m_prefix = std::string{prefix}; }
    void setPrefix(std::string &&prefix) { m_prefix = std::move(prefix); }

private:
    std::string m_prefix;
};

template<const char *Tprefix, typename Taccessor, const char *Tguilib_color = espgui::colors::GREY>
class TextWithHighlightedValueHelper : public Taccessor, public virtual TextInterface
{
    using Taccessor::Taccessor;

public:
    [[nodiscard]] std::string text() const override
    {
        using cpputils::toString;
        using espchrono::toString;
        using espcpputils::toString;
        using wifi_stack::toString;

        return std::format("{} {}{}", Tprefix, Tguilib_color, richTextEscape(toString(Taccessor::getValue())));
    }
};

template<typename Taccessor, const char *Tguilib_color = espgui::colors::GREY>
class ChangeableTextWithHighlightedValueHelper : public Taccessor, public virtual TextInterface
{
    using Taccessor::Taccessor;

public:
    [[nodiscard]] std::string text() const override
    {
        using cpputils::toString;
        using espchrono::toString;
        using espcpputils::toString;
        using wifi_stack::toString;

        return std::format("{} {}{}", m_prefix, Tguilib_color, richTextEscape(toString(Taccessor::getValue())));
    }

    [[nodiscard]] const std::string &prefix() const { return m_prefix; }
    void setPrefix(const std::string_view prefix) { m_prefix = std::string{prefix}; }
    void setPrefix(std::string &&prefix) { m_prefix = std::move(prefix); }

private:
    std::string m_prefix;
};

} // namespace espgui
