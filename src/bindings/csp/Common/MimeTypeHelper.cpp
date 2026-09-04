#include "../../containers/String.h"
#include "../../utils/Handles.h"

#include "CSP/Common/MimeTypeHelper.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

EMSCRIPTEN_DECLARE_VAL_TYPE(MimeTypesPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(FileExtensionsPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(MimeTypeHelperPointer);

EMSCRIPTEN_BINDINGS(CSPMimeTypeHelper)
{
    emscripten::register_type<MimeTypesPointer>("MimeTypes | null");
    emscripten::register_type<FileExtensionsPointer>("FileExtensions | null");
    emscripten::register_type<MimeTypeHelperPointer>("MimeTypeHelper | null");

    emscripten::class_<csp::common::MimeTypes>("MimeTypes")
        .property(
            "APPLICATION_ACAD", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_ACAD; })
        .property(
            "APPLICATION_ARJ", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_ARJ; })
        .property(
            "APPLICATION_BASE64", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_BASE64; })
        .property(
            "APPLICATION_BINHEX", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_BINHEX; })
        .property(
            "APPLICATION_BOOK", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_BOOK; })
        .property(
            "APPLICATION_CDF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_CDF; })
        .property(
            "APPLICATION_CLARISCAD", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_CLARISCAD; })
        .property(
            "APPLICATION_COMMONGROUND", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_COMMONGROUND; })
        .property(
            "APPLICATION_DRAFTING", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_DRAFTING; })
        .property(
            "APPLICATION_DSPTYPE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_DSPTYPE; })
        .property(
            "APPLICATION_DXF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_DXF; })
        .property(
            "APPLICATION_ENVOY", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_ENVOY; })
        .property(
            "APPLICATION_EXCEL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_EXCEL; })
        .property(
            "APPLICATION_FREELOADER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_FREELOADER; })
        .property(
            "APPLICATION_FUTURESPLASH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_FUTURESPLASH; })
        .property(
            "APPLICATION_GNUTAR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_GNUTAR; })
        .property(
            "APPLICATION_GROUPWISE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_GROUPWISE; })
        .property(
            "APPLICATION_HLP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_HLP; })
        .property(
            "APPLICATION_HTA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_HTA; })
        .property(
            "APPLICATION_I_DEAS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_I_DEAS; })
        .property(
            "APPLICATION_INF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_INF; })
        .property(
            "APPLICATION_JAVA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_JAVA; })
        .property(
            "APPLICATION_JAVASCRIPT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_JAVASCRIPT; })
        .property(
            "APPLICATION_LHA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_LHA; })
        .property(
            "APPLICATION_LZX", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_LZX; })
        .property(
            "APPLICATION_MARC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MARC; })
        .property(
            "APPLICATION_MBEDLET", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MBEDLET; })
        .property(
            "APPLICATION_MIME", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MIME; })
        .property(
            "APPLICATION_MSPOWERPOINT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MSPOWERPOINT; })
        .property(
            "APPLICATION_MSWORD", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MSWORD; })
        .property(
            "APPLICATION_MSWRITE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_MSWRITE; })
        .property(
            "APPLICATION_NETMC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_NETMC; })
        .property(
            "APPLICATION_OCTET_STREAM", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_OCTET_STREAM; })
        .property(
            "APPLICATION_ODA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_ODA; })
        .property(
            "APPLICATION_PDF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PDF; })
        .property(
            "APPLICATION_PKCS10", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKCS10; })
        .property(
            "APPLICATION_PKCS7_MIME", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKCS7_MIME; })
        .property(
            "APPLICATION_PKCS7_SIGNATURE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKCS7_SIGNATURE; })
        .property(
            "APPLICATION_PKCS_12", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKCS_12; })
        .property(
            "APPLICATION_PKCS_CRL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKCS_CRL; })
        .property(
            "APPLICATION_PKIX_CERT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PKIX_CERT; })
        .property(
            "APPLICATION_POSTSCRIPT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_POSTSCRIPT; })
        .property(
            "APPLICATION_POWERPOINT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_POWERPOINT; })
        .property(
            "APPLICATION_PRO_ENG", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_PRO_ENG; })
        .property(
            "APPLICATION_RINGING_TONES", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_RINGING_TONES; })
        .property(
            "APPLICATION_RTF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_RTF; })
        .property(
            "APPLICATION_SDP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SDP; })
        .property(
            "APPLICATION_SEA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SEA; })
        .property(
            "APPLICATION_SET", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SET; })
        .property(
            "APPLICATION_SLA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SLA; })
        .property(
            "APPLICATION_SMIL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SMIL; })
        .property(
            "APPLICATION_SOLIDS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SOLIDS; })
        .property(
            "APPLICATION_SOUNDER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_SOUNDER; })
        .property(
            "APPLICATION_STEP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_STEP; })
        .property(
            "APPLICATION_STREAMINGMEDIA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_STREAMINGMEDIA; })
        .property(
            "APPLICATION_TOOLBOOK", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_TOOLBOOK; })
        .property(
            "APPLICATION_VDA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VDA; })
        .property(
            "APPLICATION_VND_FDF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_FDF; })
        .property(
            "APPLICATION_VND_HP_HPGL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_HP_HPGL; })
        .property(
            "APPLICATION_VND_MS_PKI_CERTSTORE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_MS_PKI_CERTSTORE; })
        .property(
            "APPLICATION_VND_MS_PKI_PKO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_MS_PKI_PKO; })
        .property(
            "APPLICATION_VND_MS_PKI_SECCAT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_MS_PKI_SECCAT; })
        .property(
            "APPLICATION_VND_MS_POWERPOINT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_MS_POWERPOINT; })
        .property(
            "APPLICATION_VND_MS_PROJECT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_MS_PROJECT; })
        .property(
            "APPLICATION_VND_NOKIA_CONFIGURATION_MESSAGE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_NOKIA_CONFIGURATION_MESSAGE; })
        .property(
            "APPLICATION_VND_RN_REALPLAYER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_RN_REALPLAYER; })
        .property(
            "APPLICATION_VND_WAP_WMLC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_WAP_WMLC; })
        .property(
            "APPLICATION_VND_WAP_WMLSCRIPTC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_WAP_WMLSCRIPTC; })
        .property(
            "APPLICATION_VND_XARA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VND_XARA; })
        .property(
            "APPLICATION_VOCALTEC_MEDIA_DESC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VOCALTEC_MEDIA_DESC; })
        .property(
            "APPLICATION_VOCALTEC_MEDIA_FILE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_VOCALTEC_MEDIA_FILE; })
        .property(
            "APPLICATION_WORDPERFECT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_WORDPERFECT; })
        .property(
            "APPLICATION_WORDPERFECT6_0", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_WORDPERFECT6_0; })
        .property(
            "APPLICATION_WORDPERFECT6_1", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_WORDPERFECT6_1; })
        .property(
            "APPLICATION_X_123", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_123; })
        .property(
            "APPLICATION_X_AIM", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_AIM; })
        .property(
            "APPLICATION_X_AUTHORWARE_BIN", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_AUTHORWARE_BIN; })
        .property(
            "APPLICATION_X_AUTHORWARE_MAPZ", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_AUTHORWARE_MAPZ; })
        .property(
            "APPLICATION_X_AUTHORWARE_SEG", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_AUTHORWARE_SEG; })
        .property(
            "APPLICATION_X_BCPIO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_BCPIO; })
        .property(
            "APPLICATION_X_BSH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_BSH; })
        .property(
            "APPLICATION_X_BYTECODE_PYTHON", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_BYTECODE_PYTHON; })
        .property(
            "APPLICATION_X_BZIP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_BZIP; })
        .property(
            "APPLICATION_X_BZIP2", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_BZIP2; })
        .property(
            "APPLICATION_X_CDLINK", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CDLINK; })
        .property(
            "APPLICATION_X_CHAT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CHAT; })
        .property(
            "APPLICATION_X_COCOA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_COCOA; })
        .property(
            "APPLICATION_X_COMPRESSED", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_COMPRESSED; })
        .property(
            "APPLICATION_X_CONFERENCE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CONFERENCE; })
        .property(
            "APPLICATION_X_CPIO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CPIO; })
        .property(
            "APPLICATION_X_CPT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CPT; })
        .property(
            "APPLICATION_X_CSH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_CSH; })
        .property(
            "APPLICATION_X_DEEPV", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_DEEPV; })
        .property(
            "APPLICATION_X_DIRECTOR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_DIRECTOR; })
        .property(
            "APPLICATION_X_DVI", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_DVI; })
        .property(
            "APPLICATION_X_ELC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_ELC; })
        .property(
            "APPLICATION_X_ENVOY", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_ENVOY; })
        .property(
            "APPLICATION_X_ESREHBER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_ESREHBER; })
        .property(
            "APPLICATION_X_EXCEL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_EXCEL; })
        .property(
            "APPLICATION_X_FREELANCE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_FREELANCE; })
        .property(
            "APPLICATION_X_GSP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_GSP; })
        .property(
            "APPLICATION_X_GSS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_GSS; })
        .property(
            "APPLICATION_X_GTAR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_GTAR; })
        .property(
            "APPLICATION_X_HDF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_HDF; })
        .property(
            "APPLICATION_X_HELPFILE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_HELPFILE; })
        .property(
            "APPLICATION_X_HTTPD_IMAP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_HTTPD_IMAP; })
        .property(
            "APPLICATION_X_IMA", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_IMA; })
        .property(
            "APPLICATION_X_INTERNETT_SIGNUP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_INTERNETT_SIGNUP; })
        .property(
            "APPLICATION_X_INVENTOR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_INVENTOR; })
        .property(
            "APPLICATION_X_IP2", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_IP2; })
        .property(
            "APPLICATION_X_JAVA_COMMERCE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_JAVA_COMMERCE; })
        .property(
            "APPLICATION_X_KOAN", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_KOAN; })
        .property(
            "APPLICATION_X_KSH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_KSH; })
        .property(
            "APPLICATION_X_LATEX", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LATEX; })
        .property(
            "APPLICATION_X_LISP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LISP; })
        .property(
            "APPLICATION_X_LIVESCREEN", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LIVESCREEN; })
        .property(
            "APPLICATION_X_LOTUS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LOTUS; })
        .property(
            "APPLICATION_X_LOTUSSCREENCAM", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LOTUSSCREENCAM; })
        .property(
            "APPLICATION_X_LZH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_LZH; })
        .property(
            "APPLICATION_X_MAGIC_CAP_PACKAGE_1_0", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_MAGIC_CAP_PACKAGE_1_0; })
        .property(
            "APPLICATION_X_MATHCAD", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_MATHCAD; })
        .property(
            "APPLICATION_X_MIF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_MIF; })
        .property(
            "APPLICATION_X_MIX_TRANSFER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_MIX_TRANSFER; })
        .property(
            "APPLICATION_X_MPLAYER2", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_MPLAYER2; })
        .property(
            "APPLICATION_X_NAVI_ANIMATION", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NAVI_ANIMATION; })
        .property(
            "APPLICATION_X_NAVIDOC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NAVIDOC; })
        .property(
            "APPLICATION_X_NAVIMAP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NAVIMAP; })
        .property(
            "APPLICATION_X_NETCDF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NETCDF; })
        .property(
            "APPLICATION_X_NEWTON_COMPATIBLE_PKG", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NEWTON_COMPATIBLE_PKG; })
        .property(
            "APPLICATION_X_NOKIA_9000_COMMUNICATOR_ADD_ON_SOFTWARE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_NOKIA_9000_COMMUNICATOR_ADD_ON_SOFTWARE; })
        .property(
            "APPLICATION_X_OMC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_OMC; })
        .property(
            "APPLICATION_X_OMCDATAMAKER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_OMCDATAMAKER; })
        .property(
            "APPLICATION_X_OMCREGERATOR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_OMCREGERATOR; })
        .property(
            "APPLICATION_X_PAGEMAKER", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PAGEMAKER; })
        .property(
            "APPLICATION_X_PCL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PCL; })
        .property(
            "APPLICATION_X_PIXCLSCRIPT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PIXCLSCRIPT; })
        .property(
            "APPLICATION_X_PKCS7_CERTREQRESP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PKCS7_CERTREQRESP; })
        .property(
            "APPLICATION_X_PKCS7_SIGNATURE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PKCS7_SIGNATURE; })
        .property(
            "APPLICATION_X_POINTPLUS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_POINTPLUS; })
        .property(
            "APPLICATION_X_PROJECT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_PROJECT; })
        .property(
            "APPLICATION_X_QPRO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_QPRO; })
        .property(
            "APPLICATION_X_SEELOGO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SEELOGO; })
        .property(
            "APPLICATION_X_SH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SH; })
        .property(
            "APPLICATION_X_SHAR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SHAR; })
        .property(
            "APPLICATION_X_SHOCKWAVE_FLASH", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SHOCKWAVE_FLASH; })
        .property(
            "APPLICATION_X_SIT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SIT; })
        .property(
            "APPLICATION_X_SPRITE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SPRITE; })
        .property(
            "APPLICATION_X_SV4CPIO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SV4CPIO; })
        .property(
            "APPLICATION_X_SV4CRC", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_SV4CRC; })
        .property(
            "APPLICATION_X_TAR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TAR; })
        .property(
            "APPLICATION_X_TBOOK", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TBOOK; })
        .property(
            "APPLICATION_X_TCL", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TCL; })
        .property(
            "APPLICATION_X_TEX", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TEX; })
        .property(
            "APPLICATION_X_TEXINFO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TEXINFO; })
        .property(
            "APPLICATION_X_TROFF", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TROFF; })
        .property(
            "APPLICATION_X_TROFF_MAN", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TROFF_MAN; })
        .property(
            "APPLICATION_X_TROFF_ME", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TROFF_ME; })
        .property(
            "APPLICATION_X_TROFF_MS", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_TROFF_MS; })
        .property(
            "APPLICATION_X_USTAR", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_USTAR; })
        .property(
            "APPLICATION_X_VISIO", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_VISIO; })
        .property(
            "APPLICATION_X_VND_AUDIOEXPLOSION_MZZ", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_VND_AUDIOEXPLOSION_MZZ; })
        .property(
            "APPLICATION_X_VND_LS_XPIX", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_VND_LS_XPIX; })
        .property(
            "APPLICATION_X_WAIS_SOURCE", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_WAIS_SOURCE; })
        .property(
            "APPLICATION_X_WINTALK", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_WINTALK; })
        .property(
            "APPLICATION_X_WORLD", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_WORLD; })
        .property(
            "APPLICATION_X_X509_CA_CERT", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_X_X509_CA_CERT; })
        .property(
            "APPLICATION_XML", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_XML; })
        .property(
            "APPLICATION_ZIP", +[](const csp::common::MimeTypes& self) { return self.APPLICATION_ZIP; })
        .property(
            "AUDIO_AIFF", +[](const csp::common::MimeTypes& self) { return self.AUDIO_AIFF; })
        .property(
            "AUDIO_BASIC", +[](const csp::common::MimeTypes& self) { return self.AUDIO_BASIC; })
        .property(
            "AUDIO_IT", +[](const csp::common::MimeTypes& self) { return self.AUDIO_IT; })
        .property(
            "AUDIO_MAKE", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MAKE; })
        .property(
            "AUDIO_MID", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MID; })
        .property(
            "AUDIO_MIDI", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MIDI; })
        .property(
            "AUDIO_MOD", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MOD; })
        .property(
            "AUDIO_MPEG", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MPEG; })
        .property(
            "AUDIO_MPEG3", +[](const csp::common::MimeTypes& self) { return self.AUDIO_MPEG3; })
        .property(
            "AUDIO_NSPAUDIO", +[](const csp::common::MimeTypes& self) { return self.AUDIO_NSPAUDIO; })
        .property(
            "AUDIO_S3M", +[](const csp::common::MimeTypes& self) { return self.AUDIO_S3M; })
        .property(
            "AUDIO_TSP_AUDIO", +[](const csp::common::MimeTypes& self) { return self.AUDIO_TSP_AUDIO; })
        .property(
            "AUDIO_VND_QCELP", +[](const csp::common::MimeTypes& self) { return self.AUDIO_VND_QCELP; })
        .property(
            "AUDIO_VOC", +[](const csp::common::MimeTypes& self) { return self.AUDIO_VOC; })
        .property(
            "AUDIO_VOXWARE", +[](const csp::common::MimeTypes& self) { return self.AUDIO_VOXWARE; })
        .property(
            "AUDIO_WAV", +[](const csp::common::MimeTypes& self) { return self.AUDIO_WAV; })
        .property(
            "AUDIO_X_GSM", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_GSM; })
        .property(
            "AUDIO_X_JAM", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_JAM; })
        .property(
            "AUDIO_X_LIVEAUDIO", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_LIVEAUDIO; })
        .property(
            "AUDIO_X_MPEQURL", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_MPEQURL; })
        .property(
            "AUDIO_X_PN_REALAUDIO", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_PN_REALAUDIO; })
        .property(
            "AUDIO_X_PN_REALAUDIO_PLUGIN", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_PN_REALAUDIO_PLUGIN; })
        .property(
            "AUDIO_X_PSID", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_PSID; })
        .property(
            "AUDIO_X_REALAUDIO", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_REALAUDIO; })
        .property(
            "AUDIO_X_TWINVQ", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_TWINVQ; })
        .property(
            "AUDIO_X_TWINVQ_PLUGIN", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_TWINVQ_PLUGIN; })
        .property(
            "AUDIO_X_VND_AUDIOEXPLOSION_MJUICEMEDIAFILE", +[](const csp::common::MimeTypes& self) { return self.AUDIO_X_VND_AUDIOEXPLOSION_MJUICEMEDIAFILE; })
        .property(
            "AUDIO_XM", +[](const csp::common::MimeTypes& self) { return self.AUDIO_XM; })
        .property(
            "CHEMICAL_X_PDB", +[](const csp::common::MimeTypes& self) { return self.CHEMICAL_X_PDB; })
        .property(
            "I_WORLD_I_VRML", +[](const csp::common::MimeTypes& self) { return self.I_WORLD_I_VRML; })
        .property(
            "IMAGE_BMP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_BMP; })
        .property(
            "IMAGE_CMU_RASTER", +[](const csp::common::MimeTypes& self) { return self.IMAGE_CMU_RASTER; })
        .property(
            "IMAGE_FIF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_FIF; })
        .property(
            "IMAGE_FLORIAN", +[](const csp::common::MimeTypes& self) { return self.IMAGE_FLORIAN; })
        .property(
            "IMAGE_G3FAX", +[](const csp::common::MimeTypes& self) { return self.IMAGE_G3FAX; })
        .property(
            "IMAGE_GIF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_GIF; })
        .property(
            "IMAGE_IEF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_IEF; })
        .property(
            "IMAGE_JPEG", +[](const csp::common::MimeTypes& self) { return self.IMAGE_JPEG; })
        .property(
            "IMAGE_JUTVISION", +[](const csp::common::MimeTypes& self) { return self.IMAGE_JUTVISION; })
        .property(
            "IMAGE_NAPLPS", +[](const csp::common::MimeTypes& self) { return self.IMAGE_NAPLPS; })
        .property(
            "IMAGE_PICT", +[](const csp::common::MimeTypes& self) { return self.IMAGE_PICT; })
        .property(
            "IMAGE_PNG", +[](const csp::common::MimeTypes& self) { return self.IMAGE_PNG; })
        .property(
            "IMAGE_TIFF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_TIFF; })
        .property(
            "IMAGE_VND_DWG", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_DWG; })
        .property(
            "IMAGE_VND_FPX", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_FPX; })
        .property(
            "IMAGE_VND_RN_REALFLASH", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_RN_REALFLASH; })
        .property(
            "IMAGE_VND_RN_REALPIX", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_RN_REALPIX; })
        .property(
            "IMAGE_VND_WAP_WBMP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_WAP_WBMP; })
        .property(
            "IMAGE_VND_XIFF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_VND_XIFF; })
        .property(
            "IMAGE_X_ICON", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_ICON; })
        .property(
            "IMAGE_X_JG", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_JG; })
        .property(
            "IMAGE_X_JPS", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_JPS; })
        .property(
            "IMAGE_X_NIFF", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_NIFF; })
        .property(
            "IMAGE_X_PCX", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PCX; })
        .property(
            "IMAGE_X_PICT", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PICT; })
        .property(
            "IMAGE_X_PORDIV_ANYMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PORDIV_ANYMAP; })
        .property(
            "IMAGE_X_PORDIV_BITMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PORDIV_BITMAP; })
        .property(
            "IMAGE_X_PORDIV_GRAYMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PORDIV_GRAYMAP; })
        .property(
            "IMAGE_X_PORDIV_PIXMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_PORDIV_PIXMAP; })
        .property(
            "IMAGE_X_QUICKTIME", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_QUICKTIME; })
        .property(
            "IMAGE_X_RGB", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_RGB; })
        .property(
            "IMAGE_X_XBITMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_XBITMAP; })
        .property(
            "IMAGE_X_XPIXMAP", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_XPIXMAP; })
        .property(
            "IMAGE_X_XWD", +[](const csp::common::MimeTypes& self) { return self.IMAGE_X_XWD; })
        .property(
            "MESSAGE_RFC822", +[](const csp::common::MimeTypes& self) { return self.MESSAGE_RFC822; })
        .property(
            "MODEL_GLTF_BINARY", +[](const csp::common::MimeTypes& self) { return self.MODEL_GLTF_BINARY; })
        .property(
            "MODEL_GLTF_JSON", +[](const csp::common::MimeTypes& self) { return self.MODEL_GLTF_JSON; })
        .property(
            "MODEL_IGES", +[](const csp::common::MimeTypes& self) { return self.MODEL_IGES; })
        .property(
            "MODEL_VND_DWF", +[](const csp::common::MimeTypes& self) { return self.MODEL_VND_DWF; })
        .property(
            "MODEL_VND_USDZ_ZIP", +[](const csp::common::MimeTypes& self) { return self.MODEL_VND_USDZ_ZIP; })
        .property(
            "MODEL_VRML", +[](const csp::common::MimeTypes& self) { return self.MODEL_VRML; })
        .property(
            "MODEL_X_POV", +[](const csp::common::MimeTypes& self) { return self.MODEL_X_POV; })
        .property(
            "MULTIPART_X_GZIP", +[](const csp::common::MimeTypes& self) { return self.MULTIPART_X_GZIP; })
        .property(
            "PALEOVU_X_PV", +[](const csp::common::MimeTypes& self) { return self.PALEOVU_X_PV; })
        .property(
            "TEXT_ASP", +[](const csp::common::MimeTypes& self) { return self.TEXT_ASP; })
        .property(
            "TEXT_HTML", +[](const csp::common::MimeTypes& self) { return self.TEXT_HTML; })
        .property(
            "TEXT_MCF", +[](const csp::common::MimeTypes& self) { return self.TEXT_MCF; })
        .property(
            "TEXT_PASCAL", +[](const csp::common::MimeTypes& self) { return self.TEXT_PASCAL; })
        .property(
            "TEXT_PLAIN", +[](const csp::common::MimeTypes& self) { return self.TEXT_PLAIN; })
        .property(
            "TEXT_RICHTEXT", +[](const csp::common::MimeTypes& self) { return self.TEXT_RICHTEXT; })
        .property(
            "TEXT_SCRIPLET", +[](const csp::common::MimeTypes& self) { return self.TEXT_SCRIPLET; })
        .property(
            "TEXT_SGML", +[](const csp::common::MimeTypes& self) { return self.TEXT_SGML; })
        .property(
            "TEXT_TAB_SEPARATED_VALUES", +[](const csp::common::MimeTypes& self) { return self.TEXT_TAB_SEPARATED_VALUES; })
        .property(
            "TEXT_URI_LIST", +[](const csp::common::MimeTypes& self) { return self.TEXT_URI_LIST; })
        .property(
            "TEXT_VND_ABC", +[](const csp::common::MimeTypes& self) { return self.TEXT_VND_ABC; })
        .property(
            "TEXT_VND_FMI_FLEXSTOR", +[](const csp::common::MimeTypes& self) { return self.TEXT_VND_FMI_FLEXSTOR; })
        .property(
            "TEXT_VND_WAP_WML", +[](const csp::common::MimeTypes& self) { return self.TEXT_VND_WAP_WML; })
        .property(
            "TEXT_VND_WAP_WMLSCRIPT", +[](const csp::common::MimeTypes& self) { return self.TEXT_VND_WAP_WMLSCRIPT; })
        .property(
            "TEXT_WEBVIEWHTML", +[](const csp::common::MimeTypes& self) { return self.TEXT_WEBVIEWHTML; })
        .property(
            "TEXT_X_ASM", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_ASM; })
        .property(
            "TEXT_X_AUDIOSOFT_INTRA", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_AUDIOSOFT_INTRA; })
        .property(
            "TEXT_X_C", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_C; })
        .property(
            "TEXT_X_COMPONENT", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_COMPONENT; })
        .property(
            "TEXT_X_FORTRAN", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_FORTRAN; })
        .property(
            "TEXT_X_JAVA_SOURCE", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_JAVA_SOURCE; })
        .property(
            "TEXT_X_LA_ASF", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_LA_ASF; })
        .property(
            "TEXT_X_PASCAL", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_PASCAL; })
        .property(
            "TEXT_X_SCRIPT", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT; })
        .property(
            "TEXT_X_SCRIPT_ELISP", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT_ELISP; })
        .property(
            "TEXT_X_SCRIPT_PHYTON", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT_PHYTON; })
        .property(
            "TEXT_X_SCRIPT_REXX", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT_REXX; })
        .property(
            "TEXT_X_SCRIPT_TCSH", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT_TCSH; })
        .property(
            "TEXT_X_SCRIPT_ZSH", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SCRIPT_ZSH; })
        .property(
            "TEXT_X_SERVER_PARSED_HTML", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SERVER_PARSED_HTML; })
        .property(
            "TEXT_X_SETEXT", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SETEXT; })
        .property(
            "TEXT_X_SPEECH", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_SPEECH; })
        .property(
            "TEXT_X_UIL", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_UIL; })
        .property(
            "TEXT_X_UUENCODE", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_UUENCODE; })
        .property(
            "TEXT_X_VCALENDAR", +[](const csp::common::MimeTypes& self) { return self.TEXT_X_VCALENDAR; })
        .property(
            "VIDEO_ANIMAFLEX", +[](const csp::common::MimeTypes& self) { return self.VIDEO_ANIMAFLEX; })
        .property(
            "VIDEO_AVI", +[](const csp::common::MimeTypes& self) { return self.VIDEO_AVI; })
        .property(
            "VIDEO_AVS_VIDEO", +[](const csp::common::MimeTypes& self) { return self.VIDEO_AVS_VIDEO; })
        .property(
            "VIDEO_DL", +[](const csp::common::MimeTypes& self) { return self.VIDEO_DL; })
        .property(
            "VIDEO_FLI", +[](const csp::common::MimeTypes& self) { return self.VIDEO_FLI; })
        .property(
            "VIDEO_GL", +[](const csp::common::MimeTypes& self) { return self.VIDEO_GL; })
        .property(
            "VIDEO_MP4", +[](const csp::common::MimeTypes& self) { return self.VIDEO_MP4; })
        .property(
            "VIDEO_MPEG", +[](const csp::common::MimeTypes& self) { return self.VIDEO_MPEG; })
        .property(
            "VIDEO_QUICKTIME", +[](const csp::common::MimeTypes& self) { return self.VIDEO_QUICKTIME; })
        .property(
            "VIDEO_VDO", +[](const csp::common::MimeTypes& self) { return self.VIDEO_VDO; })
        .property(
            "VIDEO_VIVO", +[](const csp::common::MimeTypes& self) { return self.VIDEO_VIVO; })
        .property(
            "VIDEO_VND_RN_REALVIDEO", +[](const csp::common::MimeTypes& self) { return self.VIDEO_VND_RN_REALVIDEO; })
        .property(
            "VIDEO_VOSAIC", +[](const csp::common::MimeTypes& self) { return self.VIDEO_VOSAIC; })
        .property(
            "VIDEO_X_AMT_DEMORUN", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_AMT_DEMORUN; })
        .property(
            "VIDEO_X_AMT_SHOWRUN", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_AMT_SHOWRUN; })
        .property(
            "VIDEO_X_ATOMIC3D_FEATURE", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_ATOMIC3D_FEATURE; })
        .property(
            "VIDEO_X_DV", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_DV; })
        .property(
            "VIDEO_X_ISVIDEO", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_ISVIDEO; })
        .property(
            "VIDEO_X_MOTION_JPEG", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_MOTION_JPEG; })
        .property(
            "VIDEO_X_MS_ASF", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_MS_ASF; })
        .property(
            "VIDEO_X_QTC", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_QTC; })
        .property(
            "VIDEO_X_SCM", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_SCM; })
        .property(
            "VIDEO_X_SGI_MOVIE", +[](const csp::common::MimeTypes& self) { return self.VIDEO_X_SGI_MOVIE; })
        .property(
            "WINDOWS_METAFILE", +[](const csp::common::MimeTypes& self) { return self.WINDOWS_METAFILE; })
        .property(
            "WWW_MIME", +[](const csp::common::MimeTypes& self) { return self.WWW_MIME; })
        .property(
            "X_CONFERENCE_X_COOLTALK", +[](const csp::common::MimeTypes& self) { return self.X_CONFERENCE_X_COOLTALK; })
        .property(
            "X_WORLD_X_3DMF", +[](const csp::common::MimeTypes& self) { return self.X_WORLD_X_3DMF; })
        .property(
            "X_WORLD_X_VRT", +[](const csp::common::MimeTypes& self) { return self.X_WORLD_X_VRT; })
        .property("XGL_DRAWING", +[](const csp::common::MimeTypes& self) { return self.XGL_DRAWING; });

    emscripten::class_<csp::common::FileExtensions>("FileExtensions")
        .property(
            "_3DM", +[](const csp::common::FileExtensions& self) { return self._3DM; })
        .property(
            "_3DMF", +[](const csp::common::FileExtensions& self) { return self._3DMF; })
        .property(
            "A", +[](const csp::common::FileExtensions& self) { return self.A; })
        .property(
            "AAB", +[](const csp::common::FileExtensions& self) { return self.AAB; })
        .property(
            "AAM", +[](const csp::common::FileExtensions& self) { return self.AAM; })
        .property(
            "AAS", +[](const csp::common::FileExtensions& self) { return self.AAS; })
        .property(
            "ABC", +[](const csp::common::FileExtensions& self) { return self.ABC; })
        .property(
            "ACGI", +[](const csp::common::FileExtensions& self) { return self.ACGI; })
        .property(
            "AFL", +[](const csp::common::FileExtensions& self) { return self.AFL; })
        .property(
            "AI", +[](const csp::common::FileExtensions& self) { return self.AI; })
        .property(
            "AIF", +[](const csp::common::FileExtensions& self) { return self.AIF; })
        .property(
            "AIFC", +[](const csp::common::FileExtensions& self) { return self.AIFC; })
        .property(
            "AIFF", +[](const csp::common::FileExtensions& self) { return self.AIFF; })
        .property(
            "AIM", +[](const csp::common::FileExtensions& self) { return self.AIM; })
        .property(
            "AIP", +[](const csp::common::FileExtensions& self) { return self.AIP; })
        .property(
            "ANI", +[](const csp::common::FileExtensions& self) { return self.ANI; })
        .property(
            "AOS", +[](const csp::common::FileExtensions& self) { return self.AOS; })
        .property(
            "APS", +[](const csp::common::FileExtensions& self) { return self.APS; })
        .property(
            "ARC", +[](const csp::common::FileExtensions& self) { return self.ARC; })
        .property(
            "ARJ", +[](const csp::common::FileExtensions& self) { return self.ARJ; })
        .property(
            "ART", +[](const csp::common::FileExtensions& self) { return self.ART; })
        .property(
            "ASF", +[](const csp::common::FileExtensions& self) { return self.ASF; })
        .property(
            "ASM", +[](const csp::common::FileExtensions& self) { return self.ASM; })
        .property(
            "ASP", +[](const csp::common::FileExtensions& self) { return self.ASP; })
        .property(
            "ASX", +[](const csp::common::FileExtensions& self) { return self.ASX; })
        .property(
            "AU", +[](const csp::common::FileExtensions& self) { return self.AU; })
        .property(
            "AVI", +[](const csp::common::FileExtensions& self) { return self.AVI; })
        .property(
            "AVS", +[](const csp::common::FileExtensions& self) { return self.AVS; })
        .property(
            "BCPIO", +[](const csp::common::FileExtensions& self) { return self.BCPIO; })
        .property(
            "BIN", +[](const csp::common::FileExtensions& self) { return self.BIN; })
        .property(
            "BM", +[](const csp::common::FileExtensions& self) { return self.BM; })
        .property(
            "BMP", +[](const csp::common::FileExtensions& self) { return self.BMP; })
        .property(
            "BOO", +[](const csp::common::FileExtensions& self) { return self.BOO; })
        .property(
            "BOOK", +[](const csp::common::FileExtensions& self) { return self.BOOK; })
        .property(
            "BOZ", +[](const csp::common::FileExtensions& self) { return self.BOZ; })
        .property(
            "BSH", +[](const csp::common::FileExtensions& self) { return self.BSH; })
        .property(
            "BZ", +[](const csp::common::FileExtensions& self) { return self.BZ; })
        .property(
            "BZ2", +[](const csp::common::FileExtensions& self) { return self.BZ2; })
        .property(
            "C", +[](const csp::common::FileExtensions& self) { return self.C; })
        .property(
            "C_PLUS_PLUS", +[](const csp::common::FileExtensions& self) { return self.C_PLUS_PLUS; })
        .property(
            "CAT", +[](const csp::common::FileExtensions& self) { return self.CAT; })
        .property(
            "CC", +[](const csp::common::FileExtensions& self) { return self.CC; })
        .property(
            "CCAD", +[](const csp::common::FileExtensions& self) { return self.CCAD; })
        .property(
            "CCO", +[](const csp::common::FileExtensions& self) { return self.CCO; })
        .property(
            "CDF", +[](const csp::common::FileExtensions& self) { return self.CDF; })
        .property(
            "CER", +[](const csp::common::FileExtensions& self) { return self.CER; })
        .property(
            "CHA", +[](const csp::common::FileExtensions& self) { return self.CHA; })
        .property(
            "CHAT", +[](const csp::common::FileExtensions& self) { return self.CHAT; })
        .property(
            "CLASS", +[](const csp::common::FileExtensions& self) { return self.CLASS; })
        .property(
            "COM", +[](const csp::common::FileExtensions& self) { return self.COM; })
        .property(
            "CONF", +[](const csp::common::FileExtensions& self) { return self.CONF; })
        .property(
            "CPIO", +[](const csp::common::FileExtensions& self) { return self.CPIO; })
        .property(
            "CPP", +[](const csp::common::FileExtensions& self) { return self.CPP; })
        .property(
            "CPT", +[](const csp::common::FileExtensions& self) { return self.CPT; })
        .property(
            "CRL", +[](const csp::common::FileExtensions& self) { return self.CRL; })
        .property(
            "CRT", +[](const csp::common::FileExtensions& self) { return self.CRT; })
        .property(
            "CSH", +[](const csp::common::FileExtensions& self) { return self.CSH; })
        .property(
            "CSS", +[](const csp::common::FileExtensions& self) { return self.CSS; })
        .property(
            "CXX", +[](const csp::common::FileExtensions& self) { return self.CXX; })
        .property(
            "DCR", +[](const csp::common::FileExtensions& self) { return self.DCR; })
        .property(
            "DEEPV", +[](const csp::common::FileExtensions& self) { return self.DEEPV; })
        .property(
            "DEF", +[](const csp::common::FileExtensions& self) { return self.DEF; })
        .property(
            "DER", +[](const csp::common::FileExtensions& self) { return self.DER; })
        .property(
            "DIF", +[](const csp::common::FileExtensions& self) { return self.DIF; })
        .property(
            "DIR", +[](const csp::common::FileExtensions& self) { return self.DIR; })
        .property(
            "DL", +[](const csp::common::FileExtensions& self) { return self.DL; })
        .property(
            "DOC", +[](const csp::common::FileExtensions& self) { return self.DOC; })
        .property(
            "DOT", +[](const csp::common::FileExtensions& self) { return self.DOT; })
        .property(
            "DP", +[](const csp::common::FileExtensions& self) { return self.DP; })
        .property(
            "DRW", +[](const csp::common::FileExtensions& self) { return self.DRW; })
        .property(
            "DUMP", +[](const csp::common::FileExtensions& self) { return self.DUMP; })
        .property(
            "DV", +[](const csp::common::FileExtensions& self) { return self.DV; })
        .property(
            "DVI", +[](const csp::common::FileExtensions& self) { return self.DVI; })
        .property(
            "DWF", +[](const csp::common::FileExtensions& self) { return self.DWF; })
        .property(
            "DWG", +[](const csp::common::FileExtensions& self) { return self.DWG; })
        .property(
            "DXF", +[](const csp::common::FileExtensions& self) { return self.DXF; })
        .property(
            "EL", +[](const csp::common::FileExtensions& self) { return self.EL; })
        .property(
            "ELC", +[](const csp::common::FileExtensions& self) { return self.ELC; })
        .property(
            "ENV", +[](const csp::common::FileExtensions& self) { return self.ENV; })
        .property(
            "EPS", +[](const csp::common::FileExtensions& self) { return self.EPS; })
        .property(
            "ES", +[](const csp::common::FileExtensions& self) { return self.ES; })
        .property(
            "ETX", +[](const csp::common::FileExtensions& self) { return self.ETX; })
        .property(
            "EVY", +[](const csp::common::FileExtensions& self) { return self.EVY; })
        .property(
            "EXE", +[](const csp::common::FileExtensions& self) { return self.EXE; })
        .property(
            "F", +[](const csp::common::FileExtensions& self) { return self.F; })
        .property(
            "F77", +[](const csp::common::FileExtensions& self) { return self.F77; })
        .property(
            "F90", +[](const csp::common::FileExtensions& self) { return self.F90; })
        .property(
            "FDF", +[](const csp::common::FileExtensions& self) { return self.FDF; })
        .property(
            "FIF", +[](const csp::common::FileExtensions& self) { return self.FIF; })
        .property(
            "FLI", +[](const csp::common::FileExtensions& self) { return self.FLI; })
        .property(
            "FLO", +[](const csp::common::FileExtensions& self) { return self.FLO; })
        .property(
            "FLX", +[](const csp::common::FileExtensions& self) { return self.FLX; })
        .property(
            "FMF", +[](const csp::common::FileExtensions& self) { return self.FMF; })
        .property(
            "FOR", +[](const csp::common::FileExtensions& self) { return self.FOR; })
        .property(
            "FPX", +[](const csp::common::FileExtensions& self) { return self.FPX; })
        .property(
            "FRL", +[](const csp::common::FileExtensions& self) { return self.FRL; })
        .property(
            "FUNK", +[](const csp::common::FileExtensions& self) { return self.FUNK; })
        .property(
            "G", +[](const csp::common::FileExtensions& self) { return self.G; })
        .property(
            "G3", +[](const csp::common::FileExtensions& self) { return self.G3; })
        .property(
            "GIF", +[](const csp::common::FileExtensions& self) { return self.GIF; })
        .property(
            "GL", +[](const csp::common::FileExtensions& self) { return self.GL; })
        .property(
            "GLB", +[](const csp::common::FileExtensions& self) { return self.GLB; })
        .property(
            "GLTF", +[](const csp::common::FileExtensions& self) { return self.GLTF; })
        .property(
            "GSD", +[](const csp::common::FileExtensions& self) { return self.GSD; })
        .property(
            "GSM", +[](const csp::common::FileExtensions& self) { return self.GSM; })
        .property(
            "GSP", +[](const csp::common::FileExtensions& self) { return self.GSP; })
        .property(
            "GSS", +[](const csp::common::FileExtensions& self) { return self.GSS; })
        .property(
            "GTAR", +[](const csp::common::FileExtensions& self) { return self.GTAR; })
        .property(
            "GZ", +[](const csp::common::FileExtensions& self) { return self.GZ; })
        .property(
            "GZIP", +[](const csp::common::FileExtensions& self) { return self.GZIP; })
        .property(
            "H", +[](const csp::common::FileExtensions& self) { return self.H; })
        .property(
            "HDF", +[](const csp::common::FileExtensions& self) { return self.HDF; })
        .property(
            "HELP", +[](const csp::common::FileExtensions& self) { return self.HELP; })
        .property(
            "HGL", +[](const csp::common::FileExtensions& self) { return self.HGL; })
        .property(
            "HH", +[](const csp::common::FileExtensions& self) { return self.HH; })
        .property(
            "HLB", +[](const csp::common::FileExtensions& self) { return self.HLB; })
        .property(
            "HLP", +[](const csp::common::FileExtensions& self) { return self.HLP; })
        .property(
            "HPG", +[](const csp::common::FileExtensions& self) { return self.HPG; })
        .property(
            "HPGL", +[](const csp::common::FileExtensions& self) { return self.HPGL; })
        .property(
            "HQX", +[](const csp::common::FileExtensions& self) { return self.HQX; })
        .property(
            "HTA", +[](const csp::common::FileExtensions& self) { return self.HTA; })
        .property(
            "HTC", +[](const csp::common::FileExtensions& self) { return self.HTC; })
        .property(
            "HTM", +[](const csp::common::FileExtensions& self) { return self.HTM; })
        .property(
            "HTML", +[](const csp::common::FileExtensions& self) { return self.HTML; })
        .property(
            "HTMLS", +[](const csp::common::FileExtensions& self) { return self.HTMLS; })
        .property(
            "HTT", +[](const csp::common::FileExtensions& self) { return self.HTT; })
        .property(
            "HTX", +[](const csp::common::FileExtensions& self) { return self.HTX; })
        .property(
            "ICE", +[](const csp::common::FileExtensions& self) { return self.ICE; })
        .property(
            "ICO", +[](const csp::common::FileExtensions& self) { return self.ICO; })
        .property(
            "IDC", +[](const csp::common::FileExtensions& self) { return self.IDC; })
        .property(
            "IEF", +[](const csp::common::FileExtensions& self) { return self.IEF; })
        .property(
            "IEFS", +[](const csp::common::FileExtensions& self) { return self.IEFS; })
        .property(
            "IGES", +[](const csp::common::FileExtensions& self) { return self.IGES; })
        .property(
            "IGS", +[](const csp::common::FileExtensions& self) { return self.IGS; })
        .property(
            "IMA", +[](const csp::common::FileExtensions& self) { return self.IMA; })
        .property(
            "IMAP", +[](const csp::common::FileExtensions& self) { return self.IMAP; })
        .property(
            "INF", +[](const csp::common::FileExtensions& self) { return self.INF; })
        .property(
            "INS", +[](const csp::common::FileExtensions& self) { return self.INS; })
        .property(
            "IP", +[](const csp::common::FileExtensions& self) { return self.IP; })
        .property(
            "ISU", +[](const csp::common::FileExtensions& self) { return self.ISU; })
        .property(
            "IT", +[](const csp::common::FileExtensions& self) { return self.IT; })
        .property(
            "IV", +[](const csp::common::FileExtensions& self) { return self.IV; })
        .property(
            "IVR", +[](const csp::common::FileExtensions& self) { return self.IVR; })
        .property(
            "IVY", +[](const csp::common::FileExtensions& self) { return self.IVY; })
        .property(
            "JAM", +[](const csp::common::FileExtensions& self) { return self.JAM; })
        .property(
            "JAV", +[](const csp::common::FileExtensions& self) { return self.JAV; })
        .property(
            "JAVA", +[](const csp::common::FileExtensions& self) { return self.JAVA; })
        .property(
            "JCM", +[](const csp::common::FileExtensions& self) { return self.JCM; })
        .property(
            "JFIF", +[](const csp::common::FileExtensions& self) { return self.JFIF; })
        .property(
            "JPE", +[](const csp::common::FileExtensions& self) { return self.JPE; })
        .property(
            "JPEG", +[](const csp::common::FileExtensions& self) { return self.JPEG; })
        .property(
            "JPG", +[](const csp::common::FileExtensions& self) { return self.JPG; })
        .property(
            "JPS", +[](const csp::common::FileExtensions& self) { return self.JPS; })
        .property(
            "JS", +[](const csp::common::FileExtensions& self) { return self.JS; })
        .property(
            "JUT", +[](const csp::common::FileExtensions& self) { return self.JUT; })
        .property(
            "KAR", +[](const csp::common::FileExtensions& self) { return self.KAR; })
        .property(
            "KSH", +[](const csp::common::FileExtensions& self) { return self.KSH; })
        .property(
            "LA", +[](const csp::common::FileExtensions& self) { return self.LA; })
        .property(
            "LAM", +[](const csp::common::FileExtensions& self) { return self.LAM; })
        .property(
            "LATEX", +[](const csp::common::FileExtensions& self) { return self.LATEX; })
        .property(
            "LHA", +[](const csp::common::FileExtensions& self) { return self.LHA; })
        .property(
            "LHX", +[](const csp::common::FileExtensions& self) { return self.LHX; })
        .property(
            "LIST", +[](const csp::common::FileExtensions& self) { return self.LIST; })
        .property(
            "LMA", +[](const csp::common::FileExtensions& self) { return self.LMA; })
        .property(
            "LOG", +[](const csp::common::FileExtensions& self) { return self.LOG; })
        .property(
            "LSP", +[](const csp::common::FileExtensions& self) { return self.LSP; })
        .property(
            "LST", +[](const csp::common::FileExtensions& self) { return self.LST; })
        .property(
            "LSX", +[](const csp::common::FileExtensions& self) { return self.LSX; })
        .property(
            "LTX", +[](const csp::common::FileExtensions& self) { return self.LTX; })
        .property(
            "LZH", +[](const csp::common::FileExtensions& self) { return self.LZH; })
        .property(
            "LZX", +[](const csp::common::FileExtensions& self) { return self.LZX; })
        .property(
            "M", +[](const csp::common::FileExtensions& self) { return self.M; })
        .property(
            "M1V", +[](const csp::common::FileExtensions& self) { return self.M1V; })
        .property(
            "M2A", +[](const csp::common::FileExtensions& self) { return self.M2A; })
        .property(
            "M2V", +[](const csp::common::FileExtensions& self) { return self.M2V; })
        .property(
            "M3U", +[](const csp::common::FileExtensions& self) { return self.M3U; })
        .property(
            "MAN", +[](const csp::common::FileExtensions& self) { return self.MAN; })
        .property(
            "MAP", +[](const csp::common::FileExtensions& self) { return self.MAP; })
        .property(
            "MAR", +[](const csp::common::FileExtensions& self) { return self.MAR; })
        .property(
            "MBD", +[](const csp::common::FileExtensions& self) { return self.MBD; })
        .property(
            "MC_DOLLAR", +[](const csp::common::FileExtensions& self) { return self.MC_DOLLAR; })
        .property(
            "MCD", +[](const csp::common::FileExtensions& self) { return self.MCD; })
        .property(
            "MCF", +[](const csp::common::FileExtensions& self) { return self.MCF; })
        .property(
            "MCP", +[](const csp::common::FileExtensions& self) { return self.MCP; })
        .property(
            "ME", +[](const csp::common::FileExtensions& self) { return self.ME; })
        .property(
            "MHT", +[](const csp::common::FileExtensions& self) { return self.MHT; })
        .property(
            "MHTML", +[](const csp::common::FileExtensions& self) { return self.MHTML; })
        .property(
            "MID", +[](const csp::common::FileExtensions& self) { return self.MID; })
        .property(
            "MIDI", +[](const csp::common::FileExtensions& self) { return self.MIDI; })
        .property(
            "MIF", +[](const csp::common::FileExtensions& self) { return self.MIF; })
        .property(
            "MIME", +[](const csp::common::FileExtensions& self) { return self.MIME; })
        .property(
            "MJF", +[](const csp::common::FileExtensions& self) { return self.MJF; })
        .property(
            "MJPG", +[](const csp::common::FileExtensions& self) { return self.MJPG; })
        .property(
            "MM", +[](const csp::common::FileExtensions& self) { return self.MM; })
        .property(
            "MME", +[](const csp::common::FileExtensions& self) { return self.MME; })
        .property(
            "MOD", +[](const csp::common::FileExtensions& self) { return self.MOD; })
        .property(
            "MOOV", +[](const csp::common::FileExtensions& self) { return self.MOOV; })
        .property(
            "MOV", +[](const csp::common::FileExtensions& self) { return self.MOV; })
        .property(
            "MOVIE", +[](const csp::common::FileExtensions& self) { return self.MOVIE; })
        .property(
            "MP2", +[](const csp::common::FileExtensions& self) { return self.MP2; })
        .property(
            "MP3", +[](const csp::common::FileExtensions& self) { return self.MP3; })
        .property(
            "MP4", +[](const csp::common::FileExtensions& self) { return self.MP4; })
        .property(
            "MPA", +[](const csp::common::FileExtensions& self) { return self.MPA; })
        .property(
            "MPC", +[](const csp::common::FileExtensions& self) { return self.MPC; })
        .property(
            "MPE", +[](const csp::common::FileExtensions& self) { return self.MPE; })
        .property(
            "MPEG", +[](const csp::common::FileExtensions& self) { return self.MPEG; })
        .property(
            "MPG", +[](const csp::common::FileExtensions& self) { return self.MPG; })
        .property(
            "MPGA", +[](const csp::common::FileExtensions& self) { return self.MPGA; })
        .property(
            "MPP", +[](const csp::common::FileExtensions& self) { return self.MPP; })
        .property(
            "MPT", +[](const csp::common::FileExtensions& self) { return self.MPT; })
        .property(
            "MPV", +[](const csp::common::FileExtensions& self) { return self.MPV; })
        .property(
            "MPX", +[](const csp::common::FileExtensions& self) { return self.MPX; })
        .property(
            "MRC", +[](const csp::common::FileExtensions& self) { return self.MRC; })
        .property(
            "MS", +[](const csp::common::FileExtensions& self) { return self.MS; })
        .property(
            "MV", +[](const csp::common::FileExtensions& self) { return self.MV; })
        .property(
            "MY", +[](const csp::common::FileExtensions& self) { return self.MY; })
        .property(
            "MZZ", +[](const csp::common::FileExtensions& self) { return self.MZZ; })
        .property(
            "NAP", +[](const csp::common::FileExtensions& self) { return self.NAP; })
        .property(
            "NAPLPS", +[](const csp::common::FileExtensions& self) { return self.NAPLPS; })
        .property(
            "NC", +[](const csp::common::FileExtensions& self) { return self.NC; })
        .property(
            "NCM", +[](const csp::common::FileExtensions& self) { return self.NCM; })
        .property(
            "NIF", +[](const csp::common::FileExtensions& self) { return self.NIF; })
        .property(
            "NIFF", +[](const csp::common::FileExtensions& self) { return self.NIFF; })
        .property(
            "NIX", +[](const csp::common::FileExtensions& self) { return self.NIX; })
        .property(
            "NSC", +[](const csp::common::FileExtensions& self) { return self.NSC; })
        .property(
            "NVD", +[](const csp::common::FileExtensions& self) { return self.NVD; })
        .property(
            "O", +[](const csp::common::FileExtensions& self) { return self.O; })
        .property(
            "ODA", +[](const csp::common::FileExtensions& self) { return self.ODA; })
        .property(
            "OMC", +[](const csp::common::FileExtensions& self) { return self.OMC; })
        .property(
            "OMCD", +[](const csp::common::FileExtensions& self) { return self.OMCD; })
        .property(
            "OMCR", +[](const csp::common::FileExtensions& self) { return self.OMCR; })
        .property(
            "P", +[](const csp::common::FileExtensions& self) { return self.P; })
        .property(
            "P10", +[](const csp::common::FileExtensions& self) { return self.P10; })
        .property(
            "P12", +[](const csp::common::FileExtensions& self) { return self.P12; })
        .property(
            "P7A", +[](const csp::common::FileExtensions& self) { return self.P7A; })
        .property(
            "P7C", +[](const csp::common::FileExtensions& self) { return self.P7C; })
        .property(
            "P7M", +[](const csp::common::FileExtensions& self) { return self.P7M; })
        .property(
            "P7R", +[](const csp::common::FileExtensions& self) { return self.P7R; })
        .property(
            "P7S", +[](const csp::common::FileExtensions& self) { return self.P7S; })
        .property(
            "PART", +[](const csp::common::FileExtensions& self) { return self.PART; })
        .property(
            "PAS", +[](const csp::common::FileExtensions& self) { return self.PAS; })
        .property(
            "PBM", +[](const csp::common::FileExtensions& self) { return self.PBM; })
        .property(
            "PCL", +[](const csp::common::FileExtensions& self) { return self.PCL; })
        .property(
            "PCT", +[](const csp::common::FileExtensions& self) { return self.PCT; })
        .property(
            "PCX", +[](const csp::common::FileExtensions& self) { return self.PCX; })
        .property(
            "PDB", +[](const csp::common::FileExtensions& self) { return self.PDB; })
        .property(
            "PDF", +[](const csp::common::FileExtensions& self) { return self.PDF; })
        .property(
            "PFUNK", +[](const csp::common::FileExtensions& self) { return self.PFUNK; })
        .property(
            "PGM", +[](const csp::common::FileExtensions& self) { return self.PGM; })
        .property(
            "PIC", +[](const csp::common::FileExtensions& self) { return self.PIC; })
        .property(
            "PICT", +[](const csp::common::FileExtensions& self) { return self.PICT; })
        .property(
            "PKG", +[](const csp::common::FileExtensions& self) { return self.PKG; })
        .property(
            "PKO", +[](const csp::common::FileExtensions& self) { return self.PKO; })
        .property(
            "PL", +[](const csp::common::FileExtensions& self) { return self.PL; })
        .property(
            "PLX", +[](const csp::common::FileExtensions& self) { return self.PLX; })
        .property(
            "PM", +[](const csp::common::FileExtensions& self) { return self.PM; })
        .property(
            "PM4", +[](const csp::common::FileExtensions& self) { return self.PM4; })
        .property(
            "PM5", +[](const csp::common::FileExtensions& self) { return self.PM5; })
        .property(
            "PNG", +[](const csp::common::FileExtensions& self) { return self.PNG; })
        .property(
            "PNM", +[](const csp::common::FileExtensions& self) { return self.PNM; })
        .property(
            "POT", +[](const csp::common::FileExtensions& self) { return self.POT; })
        .property(
            "POV", +[](const csp::common::FileExtensions& self) { return self.POV; })
        .property(
            "PPA", +[](const csp::common::FileExtensions& self) { return self.PPA; })
        .property(
            "PPM", +[](const csp::common::FileExtensions& self) { return self.PPM; })
        .property(
            "PPS", +[](const csp::common::FileExtensions& self) { return self.PPS; })
        .property(
            "PPT", +[](const csp::common::FileExtensions& self) { return self.PPT; })
        .property(
            "PPZ", +[](const csp::common::FileExtensions& self) { return self.PPZ; })
        .property(
            "PRE", +[](const csp::common::FileExtensions& self) { return self.PRE; })
        .property(
            "PRT", +[](const csp::common::FileExtensions& self) { return self.PRT; })
        .property(
            "PS", +[](const csp::common::FileExtensions& self) { return self.PS; })
        .property(
            "PSD", +[](const csp::common::FileExtensions& self) { return self.PSD; })
        .property(
            "PVU", +[](const csp::common::FileExtensions& self) { return self.PVU; })
        .property(
            "PWZ", +[](const csp::common::FileExtensions& self) { return self.PWZ; })
        .property(
            "PY", +[](const csp::common::FileExtensions& self) { return self.PY; })
        .property(
            "PYC", +[](const csp::common::FileExtensions& self) { return self.PYC; })
        .property(
            "QCP", +[](const csp::common::FileExtensions& self) { return self.QCP; })
        .property(
            "QD3", +[](const csp::common::FileExtensions& self) { return self.QD3; })
        .property(
            "QD3D", +[](const csp::common::FileExtensions& self) { return self.QD3D; })
        .property(
            "QIF", +[](const csp::common::FileExtensions& self) { return self.QIF; })
        .property(
            "QT", +[](const csp::common::FileExtensions& self) { return self.QT; })
        .property(
            "QTC", +[](const csp::common::FileExtensions& self) { return self.QTC; })
        .property(
            "QTI", +[](const csp::common::FileExtensions& self) { return self.QTI; })
        .property(
            "QTIF", +[](const csp::common::FileExtensions& self) { return self.QTIF; })
        .property(
            "RA", +[](const csp::common::FileExtensions& self) { return self.RA; })
        .property(
            "RAM", +[](const csp::common::FileExtensions& self) { return self.RAM; })
        .property(
            "RAS", +[](const csp::common::FileExtensions& self) { return self.RAS; })
        .property(
            "RAST", +[](const csp::common::FileExtensions& self) { return self.RAST; })
        .property(
            "REXX", +[](const csp::common::FileExtensions& self) { return self.REXX; })
        .property(
            "RF", +[](const csp::common::FileExtensions& self) { return self.RF; })
        .property(
            "RGB", +[](const csp::common::FileExtensions& self) { return self.RGB; })
        .property(
            "RM", +[](const csp::common::FileExtensions& self) { return self.RM; })
        .property(
            "RMI", +[](const csp::common::FileExtensions& self) { return self.RMI; })
        .property(
            "RMM", +[](const csp::common::FileExtensions& self) { return self.RMM; })
        .property(
            "RMP", +[](const csp::common::FileExtensions& self) { return self.RMP; })
        .property(
            "RNG", +[](const csp::common::FileExtensions& self) { return self.RNG; })
        .property(
            "RNX", +[](const csp::common::FileExtensions& self) { return self.RNX; })
        .property(
            "ROFF", +[](const csp::common::FileExtensions& self) { return self.ROFF; })
        .property(
            "RP", +[](const csp::common::FileExtensions& self) { return self.RP; })
        .property(
            "RPM", +[](const csp::common::FileExtensions& self) { return self.RPM; })
        .property(
            "RT", +[](const csp::common::FileExtensions& self) { return self.RT; })
        .property(
            "RTF", +[](const csp::common::FileExtensions& self) { return self.RTF; })
        .property(
            "RTX", +[](const csp::common::FileExtensions& self) { return self.RTX; })
        .property(
            "RV", +[](const csp::common::FileExtensions& self) { return self.RV; })
        .property(
            "S", +[](const csp::common::FileExtensions& self) { return self.S; })
        .property(
            "S3M", +[](const csp::common::FileExtensions& self) { return self.S3M; })
        .property(
            "SAVEME", +[](const csp::common::FileExtensions& self) { return self.SAVEME; })
        .property(
            "SBK", +[](const csp::common::FileExtensions& self) { return self.SBK; })
        .property(
            "SCM", +[](const csp::common::FileExtensions& self) { return self.SCM; })
        .property(
            "SDML", +[](const csp::common::FileExtensions& self) { return self.SDML; })
        .property(
            "SDP", +[](const csp::common::FileExtensions& self) { return self.SDP; })
        .property(
            "SDR", +[](const csp::common::FileExtensions& self) { return self.SDR; })
        .property(
            "SEA", +[](const csp::common::FileExtensions& self) { return self.SEA; })
        .property(
            "SET", +[](const csp::common::FileExtensions& self) { return self.SET; })
        .property(
            "SGM", +[](const csp::common::FileExtensions& self) { return self.SGM; })
        .property(
            "SGML", +[](const csp::common::FileExtensions& self) { return self.SGML; })
        .property(
            "SH", +[](const csp::common::FileExtensions& self) { return self.SH; })
        .property(
            "SHAR", +[](const csp::common::FileExtensions& self) { return self.SHAR; })
        .property(
            "SHTML", +[](const csp::common::FileExtensions& self) { return self.SHTML; })
        .property(
            "SID", +[](const csp::common::FileExtensions& self) { return self.SID; })
        .property(
            "SIT", +[](const csp::common::FileExtensions& self) { return self.SIT; })
        .property(
            "SKD", +[](const csp::common::FileExtensions& self) { return self.SKD; })
        .property(
            "SKM", +[](const csp::common::FileExtensions& self) { return self.SKM; })
        .property(
            "SKP", +[](const csp::common::FileExtensions& self) { return self.SKP; })
        .property(
            "SKT", +[](const csp::common::FileExtensions& self) { return self.SKT; })
        .property(
            "SL", +[](const csp::common::FileExtensions& self) { return self.SL; })
        .property(
            "SMI", +[](const csp::common::FileExtensions& self) { return self.SMI; })
        .property(
            "SMIL", +[](const csp::common::FileExtensions& self) { return self.SMIL; })
        .property(
            "SND", +[](const csp::common::FileExtensions& self) { return self.SND; })
        .property(
            "SOL", +[](const csp::common::FileExtensions& self) { return self.SOL; })
        .property(
            "SPC", +[](const csp::common::FileExtensions& self) { return self.SPC; })
        .property(
            "SPL", +[](const csp::common::FileExtensions& self) { return self.SPL; })
        .property(
            "SPR", +[](const csp::common::FileExtensions& self) { return self.SPR; })
        .property(
            "SPRITE", +[](const csp::common::FileExtensions& self) { return self.SPRITE; })
        .property(
            "SRC", +[](const csp::common::FileExtensions& self) { return self.SRC; })
        .property(
            "SSI", +[](const csp::common::FileExtensions& self) { return self.SSI; })
        .property(
            "SSM", +[](const csp::common::FileExtensions& self) { return self.SSM; })
        .property(
            "SST", +[](const csp::common::FileExtensions& self) { return self.SST; })
        .property(
            "STEP", +[](const csp::common::FileExtensions& self) { return self.STEP; })
        .property(
            "STL", +[](const csp::common::FileExtensions& self) { return self.STL; })
        .property(
            "STP", +[](const csp::common::FileExtensions& self) { return self.STP; })
        .property(
            "SV4CPIO", +[](const csp::common::FileExtensions& self) { return self.SV4CPIO; })
        .property(
            "SV4CRC", +[](const csp::common::FileExtensions& self) { return self.SV4CRC; })
        .property(
            "SVF", +[](const csp::common::FileExtensions& self) { return self.SVF; })
        .property(
            "SVR", +[](const csp::common::FileExtensions& self) { return self.SVR; })
        .property(
            "SWF", +[](const csp::common::FileExtensions& self) { return self.SWF; })
        .property(
            "T", +[](const csp::common::FileExtensions& self) { return self.T; })
        .property(
            "TALK", +[](const csp::common::FileExtensions& self) { return self.TALK; })
        .property(
            "TAR", +[](const csp::common::FileExtensions& self) { return self.TAR; })
        .property(
            "TBK", +[](const csp::common::FileExtensions& self) { return self.TBK; })
        .property(
            "TCL", +[](const csp::common::FileExtensions& self) { return self.TCL; })
        .property(
            "TCSH", +[](const csp::common::FileExtensions& self) { return self.TCSH; })
        .property(
            "TEX", +[](const csp::common::FileExtensions& self) { return self.TEX; })
        .property(
            "TEXI", +[](const csp::common::FileExtensions& self) { return self.TEXI; })
        .property(
            "TEXINFO", +[](const csp::common::FileExtensions& self) { return self.TEXINFO; })
        .property(
            "TEXT", +[](const csp::common::FileExtensions& self) { return self.TEXT; })
        .property(
            "TGZ", +[](const csp::common::FileExtensions& self) { return self.TGZ; })
        .property(
            "TIF", +[](const csp::common::FileExtensions& self) { return self.TIF; })
        .property(
            "TIFF", +[](const csp::common::FileExtensions& self) { return self.TIFF; })
        .property(
            "TR", +[](const csp::common::FileExtensions& self) { return self.TR; })
        .property(
            "TSI", +[](const csp::common::FileExtensions& self) { return self.TSI; })
        .property(
            "TSP", +[](const csp::common::FileExtensions& self) { return self.TSP; })
        .property(
            "TSV", +[](const csp::common::FileExtensions& self) { return self.TSV; })
        .property(
            "TURBOT", +[](const csp::common::FileExtensions& self) { return self.TURBOT; })
        .property(
            "TXT", +[](const csp::common::FileExtensions& self) { return self.TXT; })
        .property(
            "UIL", +[](const csp::common::FileExtensions& self) { return self.UIL; })
        .property(
            "UNI", +[](const csp::common::FileExtensions& self) { return self.UNI; })
        .property(
            "UNIS", +[](const csp::common::FileExtensions& self) { return self.UNIS; })
        .property(
            "UNV", +[](const csp::common::FileExtensions& self) { return self.UNV; })
        .property(
            "URI", +[](const csp::common::FileExtensions& self) { return self.URI; })
        .property(
            "URIS", +[](const csp::common::FileExtensions& self) { return self.URIS; })
        .property(
            "USDZ", +[](const csp::common::FileExtensions& self) { return self.USDZ; })
        .property(
            "USTAR", +[](const csp::common::FileExtensions& self) { return self.USTAR; })
        .property(
            "UU", +[](const csp::common::FileExtensions& self) { return self.UU; })
        .property(
            "UUE", +[](const csp::common::FileExtensions& self) { return self.UUE; })
        .property(
            "VCD", +[](const csp::common::FileExtensions& self) { return self.VCD; })
        .property(
            "VCS", +[](const csp::common::FileExtensions& self) { return self.VCS; })
        .property(
            "VDA", +[](const csp::common::FileExtensions& self) { return self.VDA; })
        .property(
            "VDO", +[](const csp::common::FileExtensions& self) { return self.VDO; })
        .property(
            "VEW", +[](const csp::common::FileExtensions& self) { return self.VEW; })
        .property(
            "VIV", +[](const csp::common::FileExtensions& self) { return self.VIV; })
        .property(
            "VIVO", +[](const csp::common::FileExtensions& self) { return self.VIVO; })
        .property(
            "VMD", +[](const csp::common::FileExtensions& self) { return self.VMD; })
        .property(
            "VMF", +[](const csp::common::FileExtensions& self) { return self.VMF; })
        .property(
            "VOC", +[](const csp::common::FileExtensions& self) { return self.VOC; })
        .property(
            "VOS", +[](const csp::common::FileExtensions& self) { return self.VOS; })
        .property(
            "VOX", +[](const csp::common::FileExtensions& self) { return self.VOX; })
        .property(
            "VQE", +[](const csp::common::FileExtensions& self) { return self.VQE; })
        .property(
            "VQF", +[](const csp::common::FileExtensions& self) { return self.VQF; })
        .property(
            "VQL", +[](const csp::common::FileExtensions& self) { return self.VQL; })
        .property(
            "VRML", +[](const csp::common::FileExtensions& self) { return self.VRML; })
        .property(
            "VRT", +[](const csp::common::FileExtensions& self) { return self.VRT; })
        .property(
            "VSD", +[](const csp::common::FileExtensions& self) { return self.VSD; })
        .property(
            "VST", +[](const csp::common::FileExtensions& self) { return self.VST; })
        .property(
            "VSW", +[](const csp::common::FileExtensions& self) { return self.VSW; })
        .property(
            "W60", +[](const csp::common::FileExtensions& self) { return self.W60; })
        .property(
            "W61", +[](const csp::common::FileExtensions& self) { return self.W61; })
        .property(
            "W6W", +[](const csp::common::FileExtensions& self) { return self.W6W; })
        .property(
            "WAV", +[](const csp::common::FileExtensions& self) { return self.WAV; })
        .property(
            "WB1", +[](const csp::common::FileExtensions& self) { return self.WB1; })
        .property(
            "WBMP", +[](const csp::common::FileExtensions& self) { return self.WBMP; })
        .property(
            "WEB", +[](const csp::common::FileExtensions& self) { return self.WEB; })
        .property(
            "WIZ", +[](const csp::common::FileExtensions& self) { return self.WIZ; })
        .property(
            "WK1", +[](const csp::common::FileExtensions& self) { return self.WK1; })
        .property(
            "WMF", +[](const csp::common::FileExtensions& self) { return self.WMF; })
        .property(
            "WML", +[](const csp::common::FileExtensions& self) { return self.WML; })
        .property(
            "WMLC", +[](const csp::common::FileExtensions& self) { return self.WMLC; })
        .property(
            "WMLS", +[](const csp::common::FileExtensions& self) { return self.WMLS; })
        .property(
            "WMLSC", +[](const csp::common::FileExtensions& self) { return self.WMLSC; })
        .property(
            "WORD", +[](const csp::common::FileExtensions& self) { return self.WORD; })
        .property(
            "WP", +[](const csp::common::FileExtensions& self) { return self.WP; })
        .property(
            "WP5", +[](const csp::common::FileExtensions& self) { return self.WP5; })
        .property(
            "WP6", +[](const csp::common::FileExtensions& self) { return self.WP6; })
        .property(
            "WPD", +[](const csp::common::FileExtensions& self) { return self.WPD; })
        .property(
            "WQ1", +[](const csp::common::FileExtensions& self) { return self.WQ1; })
        .property(
            "WRI", +[](const csp::common::FileExtensions& self) { return self.WRI; })
        .property(
            "WRL", +[](const csp::common::FileExtensions& self) { return self.WRL; })
        .property(
            "WRZ", +[](const csp::common::FileExtensions& self) { return self.WRZ; })
        .property(
            "WSC", +[](const csp::common::FileExtensions& self) { return self.WSC; })
        .property(
            "WSRC", +[](const csp::common::FileExtensions& self) { return self.WSRC; })
        .property(
            "WTK", +[](const csp::common::FileExtensions& self) { return self.WTK; })
        .property(
            "XBM", +[](const csp::common::FileExtensions& self) { return self.XBM; })
        .property(
            "XDR", +[](const csp::common::FileExtensions& self) { return self.XDR; })
        .property(
            "XGZ", +[](const csp::common::FileExtensions& self) { return self.XGZ; })
        .property(
            "XIF", +[](const csp::common::FileExtensions& self) { return self.XIF; })
        .property(
            "XL", +[](const csp::common::FileExtensions& self) { return self.XL; })
        .property(
            "XLA", +[](const csp::common::FileExtensions& self) { return self.XLA; })
        .property(
            "XLB", +[](const csp::common::FileExtensions& self) { return self.XLB; })
        .property(
            "XLC", +[](const csp::common::FileExtensions& self) { return self.XLC; })
        .property(
            "XLD", +[](const csp::common::FileExtensions& self) { return self.XLD; })
        .property(
            "XLK", +[](const csp::common::FileExtensions& self) { return self.XLK; })
        .property(
            "XLL", +[](const csp::common::FileExtensions& self) { return self.XLL; })
        .property(
            "XLM", +[](const csp::common::FileExtensions& self) { return self.XLM; })
        .property(
            "XLS", +[](const csp::common::FileExtensions& self) { return self.XLS; })
        .property(
            "XLT", +[](const csp::common::FileExtensions& self) { return self.XLT; })
        .property(
            "XLV", +[](const csp::common::FileExtensions& self) { return self.XLV; })
        .property(
            "XLW", +[](const csp::common::FileExtensions& self) { return self.XLW; })
        .property(
            "XM", +[](const csp::common::FileExtensions& self) { return self.XM; })
        .property(
            "XML", +[](const csp::common::FileExtensions& self) { return self.XML; })
        .property(
            "XPIX", +[](const csp::common::FileExtensions& self) { return self.XPIX; })
        .property(
            "XPM", +[](const csp::common::FileExtensions& self) { return self.XPM; })
        .property(
            "X_PNG", +[](const csp::common::FileExtensions& self) { return self.X_PNG; })
        .property(
            "XSR", +[](const csp::common::FileExtensions& self) { return self.XSR; })
        .property(
            "XWD", +[](const csp::common::FileExtensions& self) { return self.XWD; })
        .property(
            "XYZ", +[](const csp::common::FileExtensions& self) { return self.XYZ; })
        .property(
            "Z", +[](const csp::common::FileExtensions& self) { return self.Z; })
        .property(
            "ZIP", +[](const csp::common::FileExtensions& self) { return self.ZIP; })
        .property(
            "ZOO", +[](const csp::common::FileExtensions& self) { return self.ZOO; })
        .property("ZSH", +[](const csp::common::FileExtensions& self) { return self.ZSH; });

    // This being a singleton is dumb. Should change in CSP.
    emscripten::class_<csp::common::MimeTypeHelper>("MimeTypeHelper")
        .class_function(
            "get", +[]() { return bindings::utils::NonOwningVal<MimeTypeHelperPointer>(&csp::common::MimeTypeHelper::Get()); })
        .property(
            "mimeType", +[](const csp::common::MimeTypeHelper& self) { return bindings::utils::NonOwningVal<MimeTypesPointer>(&self.MimeType); })
        .property(
            "fileExtension", +[](const csp::common::MimeTypeHelper& self) { return bindings::utils::NonOwningVal<FileExtensionsPointer>(&self.FileExtension); })
        .function("getMimeType(filePath)", &csp::common::MimeTypeHelper::GetMimeType);
}
