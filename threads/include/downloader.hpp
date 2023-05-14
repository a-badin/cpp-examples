#pragma once
#include <iostream>
#include <mutex>
#include <QString>
#include <QUrl>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;

inline std::string getPort(const QUrl& url) {
    std::string service = url.scheme().toStdString();
    int port = url.port();
    return port == -1 ? service : std::to_string(port);
}

class Downloader {
public:

    std::string download(const QString& url) {
        QUrl qurl(url);
        std::string host = qurl.host().toStdString();
        std::string port = getPort(url);

        beast::tcp_stream stream(ioc_);
        asio::ip::tcp::resolver::results_type results;
        {
            std::lock_guard _{mutex_};
            results = resolver_.resolve(host, port);
        }
        stream.connect(results);
    
        std::string path = qurl.path().isEmpty() ? "/" : qurl.path().toStdString();
        http::request<http::string_body> req{http::verb::get, path, 10};
        req.set(http::field::host, qurl.host().toStdString());

        http::write(stream, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        std::string result = res.body();
    
        beast::error_code ec;
        stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    
        if(ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};
        return result;
    }



private:
    std::mutex mutex_;
    asio::io_context ioc_;
    asio::ip::tcp::resolver resolver_{ioc_};
};
