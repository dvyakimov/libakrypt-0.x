/* Цель настоящего примера - вывести список всех доступных в библиотеке oid,
   не смотря на то, что библиотека не предоставляет доступа к внутреннему массиву oid.
   Пример использует неэкспортируемые функции.

   test-internal-oid03.c
*/

 #include <stdio.h>
 #include <stdlib.h>
 #include <ak_oid.h>

 int main( void )
{
  size_t i;
  ak_oid oid;

 /* инициализируем библиотеку */
  if( !ak_libakrypt_create( NULL )) return ak_libakrypt_destroy();

 /* находим какой-нибудь OID
    поскольку мы не знаем какой OID в дальнейшем будет первым в массиве */
  if(( oid = ak_oid_context_find_by_engine( random_generator )) == NULL ) {
    ak_error_message( ak_error_oid_engine, __func__, "cannot find a random generator");
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  };

 /* движемся по массиву oid в конец (пока не найдем строку из неопределенных значений)
    заметим, что движение в начало массива oid может завершиться попыткой доступа к неопределенной памяти */
  while(( oid->engine != undefined_engine ) && ( oid->mode != undefined_mode ))
     oid = (ak_oid)(((ak_uint8 *)oid) + sizeof( struct oid ));

 /* теперь, зная общее число oid, мы можем вычислить первый */
  oid = (ak_oid)(((ak_uint8 *)oid) - ak_libakrypt_oids_count()*sizeof( struct oid ));

 /* выводим все oid, начиная с начала */
  printf("%-40s %-16s %-14s %s\n", "name", "engine", "mode", "oid" );
  printf("----------------------------------------------------------------------------------------------\n");
  for( i = 0; i < ak_libakrypt_oids_count(); i++ ) {
     printf("%-40s %-16s %-14s %s\n",
        oid->name, ak_libakrypt_get_engine_name( oid->engine ),
                       ak_libakrypt_get_mode_name( oid->mode ), oid->id );
     oid = (ak_oid)(((ak_uint8 *)oid) + sizeof( struct oid ));
  }

 ak_libakrypt_destroy();
 return EXIT_SUCCESS;
}
