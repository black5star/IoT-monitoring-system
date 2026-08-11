build:
	docker compose build --parallel

up:
	docker compose up -d 

re: 
	docker compose stop
	$(MAKE) up

down:
	docker compose down

logs:
	docker compose logs -f

fclean: 
	docker compose down -v
	docker system prune -af
	docker volume prune -f

push: down
	git add .
	git commit -m "$(ARGS)"
	git push

.PHONY: build up re down logs fclean push
