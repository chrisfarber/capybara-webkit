#include "Reset.h"
#include "WebPage.h"
#include "NetworkAccessManager.h"
#include "NetworkCookieJar.h"

Reset::Reset(WebPage *page, QObject *parent) : Command(page, parent) {
}

void Reset::start(QStringList &arguments) {
  Q_UNUSED(arguments);

  page()->triggerAction(QWebPage::Stop);
  page()->currentFrame()->setHtml("<html><body></body></html>");

  NetworkAccessManager* networkAccessManager = qobject_cast<NetworkAccessManager*>(page()->networkAccessManager());
  networkAccessManager->setCookieJar(new NetworkCookieJar());
  networkAccessManager->resetHeaders();

  page()->setUserAgent(NULL);
  page()->resetResponseHeaders();
  page()->resetConsoleMessages();
  page()->resetSettings();
  resetHistory();
  emit finished(new Response(true));
}

void Reset::resetHistory() {
  // Clearing the history preserves the current history item, so set it to blank first.
  page()->currentFrame()->setUrl(QUrl("about:blank"));
  page()->history()->clear();
}

