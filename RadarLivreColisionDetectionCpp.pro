TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lcurl -lodb -lodb-pgsql -lpthread

SOURCES += main.cpp \
    cpr/auth.cpp \
    cpr/cookies.cpp \
    cpr/cprtypes.cpp \
    cpr/digest.cpp \
    cpr/error.cpp \
    cpr/multipart.cpp \
    cpr/parameters.cpp \
    cpr/payload.cpp \
    cpr/proxies.cpp \
    cpr/session.cpp \
    cpr/util.cpp \
    radarlivre_api_flightinfo-odb.cxx \
    radarlivre_api_observation-odb.cxx

HEADERS += \
    rlglobalposition.h \
    rlmath.h \
    rlaircraft.h \
    rlenuvector.h \
    rladsbinfo.h \
    rlvector2d.h \
    rlcylinder.h \
    rlpropagatedinfo.h \
    rlcollisiondetector.h \
    rlcollisionalert.h \
    rlcollisionalertlistener.h \
    rlrepository.h \
    rlremoterepository.h \
    rlsimulatorrepository.h \
    cpr/api.h \
    cpr/auth.h \
    cpr/body.h \
    cpr/cookies.h \
    cpr/cpr.h \
    cpr/cprtypes.h \
    cpr/curlholder.h \
    cpr/defines.h \
    cpr/digest.h \
    cpr/error.h \
    cpr/max_redirects.h \
    cpr/multipart.h \
    cpr/parameters.h \
    cpr/payload.h \
    cpr/proxies.h \
    cpr/response.h \
    cpr/session.h \
    cpr/timeout.h \
    cpr/util.h \
    json.h \
    radarlivre_api_flightinfo.h \
    radarlivre_api_observation.h \
    radarlivre_api_flightinfo-odb.hxx \
    radarlivre_api_observation-odb.hxx \
    traits-pgsql.hxx \
    rlutil.h \
    rlcombinator.h \
    rlcombinationexecutor.h \
    rlstring.h
