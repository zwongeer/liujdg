#include <restinio/all.hpp>

int main()
{
  restinio::run(
    restinio::on_this_thread()
      .port(1984)
      .address("localhost")
      .request_handler([](auto req) {
        return req->create_response().set_body("hello liujdg!").done();
      }));

  return 0;
}
