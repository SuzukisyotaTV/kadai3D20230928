# include <Siv3D.hpp>
void Main()
{
	// ブロックのサイズ
	constexpr Size brickSize{ 40, 20 };
	// ボールの速さ
	constexpr double speed = 480.0;
	// ボールの速度
	Vec2 ballVelocity{ 0, -speed };
	Vec2 ball2Velocity{ 0, -speed };
	int bardel = 100;
	int  pls = false;
	// ボール
	Circle ball{ 400, 400, 8 };
	Circle ball2{ 500, 400, 8 };
	// ブロックの配列
	Array<Rect> bricks;
	// 横 (Scene::Width() / blockSize.x) 個、縦 5 個のブロックを配列に追加する
	for (auto p : step(Size{ (Scene::Width() / brickSize.x), 5 }))
	{
		bricks << Rect{ (p.x * brickSize.x), (60 + p.y * brickSize.y), brickSize };
	}
	while (System::Update())
	{

		// パドル
		const Rect paddle{ Arg::center(Cursor::Pos().x, 500), bardel, 10 };
		// ボールを移動
		if (KeySpace.pressed()) {
			pls = true;
		}
		if (pls == true) {
			ball.moveBy(ballVelocity * Scene::DeltaTime());
			ball2.moveBy(ball2Velocity * Scene::DeltaTime());
		}
		// ブロックを順にチェック
		for (auto it = bricks.begin(); it != bricks.end(); ++it)
		{
			// ブロックとボールが交差していたら
			if (it->intersects(ball))
			{
				// ボールの向きを反転する
				(it->bottom().intersects(ball) || it->top().intersects(ball)
					? ballVelocity.y : ballVelocity.x) *= -1;
				// ブロックを配列から削除（イテレータが無効になるので注意）
				bricks.erase(it);
				// これ以上チェックしない
				break;
			}if (it->intersects(ball2))
			{
				// ボールの向きを反転する
				(it->bottom().intersects(ball2) || it->top().intersects(ball2)
					? ball2Velocity.y : ball2Velocity.x) *= -1;
				// ブロックを配列から削除（イテレータが無効になるので注意）
				bricks.erase(it);
				// これ以上チェックしない
				break;
			}
		}
		// 天井にぶつかったらはね返る
		if (ball.y < 0 && ballVelocity.y < 0)
		{
			ballVelocity.y *= -1;
		}
		if (ball2.y < 0 && ball2Velocity.y < 0)
		{
			ball2Velocity.y *= -1;
		}
		// 左右の壁にぶつかったらはね返る
		if ((ball.x < 0 && ballVelocity.x < 0)
			|| (Scene::Width() < ball.x && 0 < ballVelocity.x))
		{
			ballVelocity.x *= -1;
		}
		// 左右の壁にぶつかったらはね返る
		if ((ball2.x < 0 && ball2Velocity.x < 0)
			|| (Scene::Width() < ball2.x && 0 < ball2Velocity.x))
		{
			ball2Velocity.x *= -1;
		}
		// パドルにあたったらはね返る
		if (0 < ballVelocity.y && paddle.intersects(ball))
		{
			// パドルの中心からの距離に応じてはね返る方向を変える
			ballVelocity = Vec2{ (ball.x - paddle.center().x) * 10, -ballVelocity.y }.setLength(speed);
			bardel -= 3;
		}
		if (0 < ball2Velocity.y && paddle.intersects(ball2))
		{
			// パドルの中心からの距離に応じてはね返る方向を変える
			ball2Velocity = Vec2{ (ball2.x - paddle.center().x) * 10, -ball2Velocity.y }.setLength(speed);
			bardel -= 3;
		}
		// すべてのブロックを描画する
		for (const auto& brick : bricks)
		{
			brick.stretched(-1).draw(HSV{ brick.y - 40 });
		}
		// ボールを描く
		ball.draw();
		ball2.draw();
		// パドルを描く
		paddle.draw();
	}
}
