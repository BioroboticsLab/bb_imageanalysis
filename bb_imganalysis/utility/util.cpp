#include "util.h"

#include <pipeline/Preprocessor.h>
#include <pipeline/EllipseFitter.h>
#include <pipeline/GridFitter.h>
#include <pipeline/Localizer.h>
#include <pipeline/Decoder.h>

namespace {
void checkPath (const boost::filesystem::path& path)
{
    if (!boost::filesystem::is_regular_file(path))
    {
        throw std::runtime_error(path.string() + " is not a valid file");
    }
}
}

void util::loadSettingsFile(pipeline::Preprocessor& preprocessor,
                            pipeline::Localizer &localizer,
                            pipeline::EllipseFitter &ellipseFitter,
                            pipeline::GridFitter &gridFitter,
                            pipeline::Decoder &decoder,
                            const boost::filesystem::path &configFile)
{
    checkPath(configFile);

    pipeline::settings::preprocessor_settings_t preprocessor_settings;
    pipeline::settings::localizer_settings_t localizer_settings;
    pipeline::settings::ellipsefitter_settings_t ellipsefitter_settings;
    pipeline::settings::gridfitter_settings_t gridfitter_settings;

    for (pipeline::settings::settings_abs* settings :
         std::array<pipeline::settings::settings_abs*, 4>({&preprocessor_settings,
                                                           &localizer_settings,
                                                           &ellipsefitter_settings,
                                                           &gridfitter_settings}))
    {
        settings->loadFromJson(configFile.string());
    }

    static const boost::filesystem::path deeplocalizer_model_path(BOOST_PP_STRINGIZE(MODEL_BASE_PATH));

    boost::filesystem::path model_path(localizer_settings.get_deeplocalizer_model_file());
    model_path = deeplocalizer_model_path / model_path.parent_path().leaf() / model_path.filename();

    boost::filesystem::path param_path(localizer_settings.get_deeplocalizer_param_file());
    param_path = deeplocalizer_model_path / model_path.parent_path().leaf() / param_path.filename();

    localizer_settings.setValue(pipeline::settings::Localizer::Params::DEEPLOCALIZER_MODEL_FILE,
                                model_path.string());
    localizer_settings.setValue(pipeline::settings::Localizer::Params::DEEPLOCALIZER_PARAM_FILE,
                                param_path.string());

    checkPath(model_path);
    checkPath(param_path);

    preprocessor.loadSettings(preprocessor_settings);
    localizer.loadSettings(localizer_settings);
    ellipseFitter.loadSettings(ellipsefitter_settings);
    gridFitter.loadSettings(gridfitter_settings);
}
